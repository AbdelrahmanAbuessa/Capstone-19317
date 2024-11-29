var sheet_id = '1iIlUPezACZyGTjZ8upECTVZozjDbtc6TgOd7qhlHCYo';
var main_sheet_name = "main_sheet";
var sheet_open = SpreadsheetApp.openById(sheet_id);
var sheet_target = sheet_open.getSheetByName(main_sheet_name);
var cache = CacheService.getScriptCache();

function doGet(e) {
  Logger.log(JSON.stringify(e));
  var result = 'Ok';
  if (e.parameter == 'undefined') {
    result = 'No Parameters';
  } else {
    var newRow = sheet_target.getLastRow() + 1;
    var rowDataLog = [];

    let calender = new Date();
    let year = calender.getFullYear();
    let month = calender.getMonth() + 1;
    let day = calender.getDate();
    let hour = calender.getHours();
    let minute = calender.getMinutes();

    var current_Date = `${month}/${day}/${year}`;
    rowDataLog[0] = current_Date;

    var current_Time = `${hour}:${minute}`;
    rowDataLog[1] = current_Time;

    var sts_val = '';

    // Loop through parameters sent from ESP32
    for (var param in e.parameter) {
      Logger.log('In for loop, param=' + param);
      var value = stripQuotes(e.parameter[param]);
      Logger.log(param + ':' + e.parameter[param]);

      switch (param) {
        case 'sts':
          sts_val = value;
          break;
        case 'temp':
          rowDataLog[2] = value;
          result += ', DS18B20 added';
          break;
        case 'humid':
          rowDataLog[3] = value;
          result += ', DHT22 added';
          break;
        case 'cot':
          rowDataLog[4] = value;
          result += ', MQ-2 added';
          break;
        default:
          result += ", unsupported parameter";
      }
    }

    if (sts_val === 'write') {
      Logger.log(JSON.stringify(rowDataLog));
      var newRangeDataLog = sheet_target.getRange(newRow, 1, 1, rowDataLog.length);
      newRangeDataLog.setValues([rowDataLog]);
      SpreadsheetApp.flush(); // Ensure data is written

      // Clear cache to force refresh
      cache.remove('latestData');
      return ContentService.createTextOutput(result);
    }
  }

  // Get the template and data
  var t = HtmlService.createTemplateFromFile('index');
  t.data = fetchLatestData(); // Fetch fresh data
  return t.evaluate()
    .setTitle("BESS Dashboard")
    .setFaviconUrl("https://png.pngtree.com/png-vector/20221228/ourmid/pngtree-middle-speed-meter-in-green-yellow-red-color-for-indicator-png-image_6535104.png");
}

function postData() {
  return fetchLatestData();
}

function post20Data() {
  return fetchLatest20Data();
}

// Function to fetch latest data with cache handling
function fetchLatestData() {
  let cachedData = cache.get('latestData');
  if (cachedData) {
    Logger.log('Using cached data');
    return JSON.parse(cachedData);
  } else {
    SpreadsheetApp.flush(); // Ensure data is up-to-date
    let data = sheet_target.getDataRange().getValues().slice(2);
    let latestEntry = data.length ? data[data.length - 1] : ["Loading...", ".", "0", "0", "0"];
    let obj = {
      "date": latestEntry[0],
      "time": latestEntry[1],
      "temp": latestEntry[2],
      "humid": latestEntry[3],
      "cot": latestEntry[4],
    };
    cache.put('latestData', JSON.stringify(obj), 10);
    Logger.log('Fetched fresh data: ' + JSON.stringify(obj));
    return obj;
  }
}

function fetchLatest20Data() {
  let cachedData = cache.get('latest20Data');
  if (cachedData) {
    Logger.log('Using cached data');
    return JSON.parse(cachedData);
  } else {
    SpreadsheetApp.flush(); // Ensure data is up-to-date
    let data = sheet_target.getDataRange().getValues().slice(2);
    let latestEntries = data.length >= 20 ? data.slice(data.length - 20) : data;
    
    let result = latestEntries.map(entry => ({
      "temp": entry[2],
      "humid": entry[3],
      "cot": entry[4],
    }));

    cache.put('latest20Data', JSON.stringify(result), 10);
    Logger.log('Fetched fresh data: ' + JSON.stringify(result));
    return result;
  }
}

function stripQuotes(value) {
  return value.replace(/^["']|['"]$/g, "");
}
