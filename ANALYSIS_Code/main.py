# ---------- Import Libraries & Files START ----------
import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
db_filepath = "E:/G12/CAPSTONE/Work Process/Code/db-19317.csv"
dataframe = pd.read_csv(db_filepath)
# ---------- Import Libraries & Files END ----------

# ---------- Data Modification START ----------
filtered_dataframe = dataframe[
    (dataframe["Temp"] >= 0) &
    (dataframe["Temp"] <= 80) &
    (dataframe["CO"] < 1000) &
    (dataframe["Humid"] != 0)
]

temp_data = filtered_dataframe["Temp"]
humid_data = filtered_dataframe["Humid"]
cot_data = filtered_dataframe["CO"]
# ---------- Data Modification END ----------

# ---------- Accuracy & Precision START ----------
def calculate_standard_error(data):
    population_mean = np.mean(data)
    population_std = np.std(data, ddof=1)
    sample_size = len(data)
    standard_error = population_std / (np.sqrt(sample_size) * population_mean)
    return standard_error

temp_sem = (calculate_standard_error(temp_data) * 100).__round__(3)
humid_sem = (calculate_standard_error(humid_data) * 100).__round__(3)
cot_sem = (calculate_standard_error(cot_data) * 100).__round__(3)

print(f"Error for Temperature: {temp_sem}%")
print(f"Error for Humidity: {humid_sem}%")
print(f"Error for CO: {cot_sem}%")
# ---------- Accuracy & Precision END ----------

# ---------- Scatter Plot & Regression Line START ----------
slope, intercept = np.polyfit(temp_data, cot_data, 1)
print(f"Regression Line Equation: y = {slope:.3f}x + {intercept:.3f}")

correlation_matrix = np.corrcoef(temp_data, cot_data)
regression_coefficient = correlation_matrix[0, 1]
print(f"Regression Coefficient (Correlation) between Temperature and CO: {regression_coefficient:.3f}")

plt.figure(figsize=(10, 6))
plt.scatter(temp_data, cot_data, color='blue', alpha=0.7, label="Data Points")

regression_line = slope * temp_data + intercept
plt.plot(temp_data, regression_line, color='red', label=f"Regression Line (r)")

plt.title("Scatter Plot of Temperature vs CO with Regression Line", fontsize=14)
plt.xlabel("Temperature", fontsize=12)
plt.ylabel("CO", fontsize=12)
plt.grid(True, linestyle='--', alpha=0.6)
plt.legend()
plt.tight_layout()
plt.show()
# ---------- Scatter Plot & Regression Line END ----------