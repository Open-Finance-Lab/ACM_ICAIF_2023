import csv
import datetime
import numpy as np

data = []
with open('results.csv', 'r') as csvfile:
    reader = csv.reader(csvfile)
    next(reader)
    for row in reader:
        date = datetime.datetime.strptime(row[0], '%Y-%m-%d')
        capital = float(row[1])
        data.append((date, capital))

data.sort(key=lambda x: x[0])
data_dict = {date: capital for date, capital in data}
start_date = data[0][0]
end_date = data[-1][0]
date_range = [start_date + datetime.timedelta(days=i) for i in range((end_date - start_date).days + 1)]

returns = []
for i in range(1, len(date_range)):
    prev_date = date_range[i - 1]
    current_date = date_range[i]
    
    if prev_date in data_dict and current_date in data_dict:
        prev_capital = data_dict[prev_date]
        current_capital = data_dict[current_date]
        daily_return = (current_capital - prev_capital) / prev_capital
        returns.append(daily_return)
    else:
        # If data is missing for a date, insert a return of 0
        returns.append(0)

average_return = np.mean(returns)
std_deviation = np.std(returns)
risk_free_rate = 0.02 / 252

sharpe_ratio = (average_return - risk_free_rate) / std_deviation

print(f"Average Daily Return: {average_return:.6f}")
print(f"Standard Deviation of Daily Returns: {std_deviation:.6f}")
print(f"Sharpe Ratio: {sharpe_ratio:.6f}")
