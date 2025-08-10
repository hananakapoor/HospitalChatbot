Prerequisites

sudo apt update
sudo apt install libboost-system-dev libboost-filesystem-dev
sudo apt install libmysqlclient-dev
sudo apt install build-essential
sudo apt install g++

 Compilation and Execution
to run 
1 .g++ -std=c++17 -o server voice_chat2.cpp -lboost_system -lboost_filesystem -lpthread -lmysqlclient

2. sudo service mysql start


3. ./server


## Database Configuration

manager_ = new CallManager("newpassword", "newuser", "127.0.0.1", "3306", "ksfe_db", 10, 5);



change name to your root  and password   ;change db to your db name table name  shoud be same States and Transitions

 Table: `States`

CREATE TABLE States (
    ID INT PRIMARY KEY,
    Description VARCHAR(255),
    Message TEXT
);

INSERT INTO States (ID, Description, Message) VALUES
(1, 'Welcome', 'Welcome to KSFE! How can I assist you today?'),
(2, 'Service Selection', 'Kindly select your service: Loans or Deposits.'),
(3, 'Loan State', 'You selected Loans. Please choose the loan type: Housing Loan, Gold Loan.'),
(4, 'Loan Type Selection', 'Kindly select a loan type: Housing Loan, Gold Loan, etc.'),
(5, 'Gold Loan Questions', 'What would you like to know about Gold Loans?'),
(6, 'Housing Loan Questions', 'What would you like to know about Housing Loans?'),
(7, 'Gold Loan Interest Rate', 'The interest rate for KSFE gold loans starts at 7% per annum.'),
(8, 'Gold Loan Maximum Amount', 'The maximum loan amount depends on gold''s weight and purity, up to ₹50 lakhs.'),
(9, 'Gold Loan Documents', 'Valid ID proof, address proof, and recent photographs are required.'),
(10, 'Gold Loan Purity', 'KSFE accepts gold with a minimum purity of 18 karats.'),
(11, 'Gold Loan Tenure', 'The tenure ranges from 3 months to 36 months, depending on the scheme.'),
(12, 'Janamithram Gold Loan', 'This scheme offers lower interest rates and higher loan-to-value ratios.'),
(13, 'Premium Gold Loan', 'Offers higher loan amounts per gram of gold at competitive rates.'),
(14, 'Gold Loan EMI Options', 'KSFE offers flexible EMI options, including monthly and bullet payments.'),
(15, 'Gold Loan Early Repayment', 'Yes, early repayment is allowed without prepayment penalties.'),
(16, 'Deposit State', 'You selected Deposits. What would you like to know?'),
(17, 'Deposit Types', 'KSFE offers Fixed Deposit, Short Term Deposit, Sugama Security Deposit, and Prize Money Deposit.'),
(18, 'Fixed Deposit Interest Rate', 'The interest rate varies based on tenure, ranging from 6.5% to 7.5%.'),
(19, 'Senior Citizen Rates', 'Senior citizens get an additional 0.5% interest rate on Fixed Deposits.'),
(20, 'Short Term Deposit Minimum', 'The minimum deposit amount is ₹1,000.'),
(21, 'Prize Money Deposit', 'A scheme with periodic draws offering prize money in addition to interest.'),
(22, 'Sugama Security Deposit', 'A scheme offering guaranteed returns with flexibility in withdrawal and reinvestment.'),
(23, 'Deposit Early Withdrawal', 'Yes, deposits can be withdrawn before maturity, but penalties may apply.'),
(24, 'Short Term Deposit Duration', 'The duration ranges from 7 days to 12 months.'),
(25, 'Fixed Deposit Account', 'Visit the nearest branch with required documents or apply online.'),
(26, 'Deposit Documents', 'ID proof, address proof, PAN card, and recent photographs are required.'),
(27, 'Query Satisfaction', 'Are you satisfied with the details provided?'),
(28, 'End', 'Thank you for choosing KSFE. Have a great day!');
```

Table: `Transitions`

CREATE TABLE Transitions (
    FromState INT,
    Answer VARCHAR(255),
    ToState INT,
    PRIMARY KEY (FromState, Answer)
);

INSERT INTO Transitions (FromState, Answer, ToState) VALUES
(1, 'Service Selection', 2),
(2, 'Loan State', 3),
(2, 'Deposit State', 16),
(3, 'Gold Loan Questions', 5),
(3, 'Housing Loan Questions', 6),
(5, 'Interest Rate', 7),
(5, 'Maximum Loan Amount', 8),
(5, 'Required Documents', 9),
(5, 'Minimum Purity', 10),
(5, 'Loan Tenure', 11),
(5, 'Janamithram Gold Loan', 12),
(5, 'Premium Gold Loan', 13),
(5, 'EMI Options', 14),
(5, 'Early Repayment', 15),
(16, 'Deposit Types', 17),
(16, 'Fixed Deposit Interest Rate', 18),
(16, 'Senior Citizen Rates', 19),
(16, 'Short Term Deposit Minimum', 20),
(16, 'Prize Money Deposit', 21),
(16, 'Sugama Security Deposit', 22),
(16, 'Early Withdrawal', 23),
(16, 'Short Term Deposit Duration', 24),
(16, 'Open Fixed Deposit Account', 25),
(16, 'Deposit Documents', 26),
(27, 'Yes', 28),
(27, 'No', 2);



1. Ensure MySQL server is running and database `ksfe_db` is properly configured.

sudo service mysql start


