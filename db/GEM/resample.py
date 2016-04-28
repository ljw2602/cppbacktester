import pandas as pd

def sample_daily(name):
    df = pd.read_csv(name+".csv", index_col=0, parse_dates=0)
    df.resample('D').interpolate().to_csv(name+"_daily.csv")

names = ["USBond", "SP500TR", "ACWIexUS"]

for name in names:
    sample_daily(name)


