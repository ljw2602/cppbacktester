import numpy as np
import pandas as pd
import pandas_datareader.data as web
from datetime import datetime

etfs = ['XLY','XLP','XLE','XLF','XLV','XLI','XLB','XLK','XLU']
adjclose = pd.DataFrame()
close = pd.DataFrame()
open = pd.DataFrame()
for ticker in etfs:
    data = web.DataReader(ticker, 'yahoo', datetime(1900,1,1), datetime.today())
    data.to_csv(ticker+'.csv')
    
    adj = data['Adj Close']; adj.name = ticker;
    adjclose = pd.concat([adjclose, adj], axis=1)
    
    cl = data['Close']; cl.name = ticker;
    close = pd.concat([close, cl], axis=1)
    
    op = data['Open']; op.name = ticker;
    open = pd.concat([open, op], axis=1)

adjclose.to_csv('AdjClose.csv')
close.to_csv('Close.csv')
open.to_csv('Open.csv')

#adjclose = pd.read_csv("AdjClose.csv", index_col=0, parse_dates=0)
adjclose_eom = adjclose.resample('1M').last().interpolate('linear').dropna(axis=0)

monthly_ret = (adjclose_eom - adjclose_eom.shift(1))/adjclose_eom.shift(1)

avg_monthly_ret_annualized = monthly_ret.mean(axis=0)*12
avg_monthly_ret_annualized.to_csv('hist_ret.csv')

stdev_monthly_ret_annualized = monthly_ret.std(axis=0)*np.sqrt(12)
stdev_monthly_ret_annualized.to_csv('hist_stdev.csv')

varcov_monthly_ret_annualized = monthly_ret.cov()*12
varcov_monthly_ret_annualized.to_csv('hist_varcov.csv')

corr_monthly_ret = monthly_ret.corr()
corr_monthly_ret.to_csv('hist_corr.csv')

for ticker in etfs:
    data = web.DataReader(ticker, 'yahoo-actions', datetime(1900,1,1), datetime.today())
    data.to_csv(ticker+'_actions.csv')
