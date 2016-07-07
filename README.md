# C++ Backtester

Algorithmic trading strategy backtesting platform

Remarks:

* Corporate action data (e.g. split, dividend) using Python Datareader
* Chronological backtesting
 * Prevents look-ahead bias
 * Hudson runs asset by asset, so it sometimes travels back in time
* Daily balance settlement
 * Introduces brokerage account concept to track the account balance in dollars
 * Throws an error when the balance goes to negative after the requested execution
 * Hudson does not have this concept
* Statistics calculation
 * beta


For more detailed description of the code, please visit [here](http://jmoneylab.com/wiki/index.php?title=C%2B%2B_Backtester)
