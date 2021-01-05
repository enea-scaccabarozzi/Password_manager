use Password_manager;

/*------CREATE TABLE USER------*/

CREATE TABLE IF NOT EXISTS `user` (
  `ID` int(11) NOT NULL AUTO_INCREMENT,
  `UserName` varchar(50) NOT NULL,
  `UserMail` varchar(50) NOT NULL,
  `pwd` varchar(100) NOT NULL,
  PRIMARY KEY (ID)
);


/*-----CREATE TABLE DATA-------*/

CREATE TABLE IF NOT EXISTS `data` (
  `UserID` int(11) NOT NULL,
  `DataID` int(11) NOT NULL AUTO_INCREMENT,
  `pwd` varchar(100) NOT NULL,
  `tm` timestamp NOT NULL DEFAULT current_timestamp() ON UPDATE current_timestamp(),
  `service` varchar(50) NOT NULL,
  `account` varchar(50) NOT NULL,
  `IV` binary(16) NOT NULL,
  INDEX(DataID),
  PRIMARY KEY (UserID, DataID),
  FOREIGN KEY (UserID) REFERENCES user(ID)
);


/*--CREATE TABLE CURRENT_DATA--*/

CREATE TABLE IF NOT EXISTS `current_data` (
  `Ind` int(11) NOT NULL AUTO_INCREMENT,
  `pwd` varchar(100) NOT NULL,
  `tm` timestamp NOT NULL DEFAULT current_timestamp(),
  `service` varchar(50) NOT NULL,
  `account` varchar(50) NOT NULL,
  `bool` int(11) DEFAULT 0,
  `accountID` int(11) NOT NULL,
  PRIMARY KEY (Ind),
  FOREIGN KEY (accountID) REFERENCES user(ID)
);

