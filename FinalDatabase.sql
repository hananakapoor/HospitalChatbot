-- MySQL dump 10.13  Distrib 8.0.41, for Linux (x86_64)
--
-- Host: localhost    Database: Final
-- ------------------------------------------------------
-- Server version	8.0.41-0ubuntu0.24.04.1

/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!50503 SET NAMES utf8mb4 */;
/*!40103 SET @OLD_TIME_ZONE=@@TIME_ZONE */;
/*!40103 SET TIME_ZONE='+00:00' */;
/*!40014 SET @OLD_UNIQUE_CHECKS=@@UNIQUE_CHECKS, UNIQUE_CHECKS=0 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;
/*!40111 SET @OLD_SQL_NOTES=@@SQL_NOTES, SQL_NOTES=0 */;

--
-- Table structure for table `Booking`
--

DROP TABLE IF EXISTS `Booking`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `Booking` (
  `id` int NOT NULL AUTO_INCREMENT,
  `state` varchar(255) NOT NULL,
  `category` varchar(255) NOT NULL,
  `sub_category` varchar(255) NOT NULL,
  `message` text NOT NULL,
  `token` int DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=3 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `Booking`
--

LOCK TABLES `Booking` WRITE;
/*!40000 ALTER TABLE `Booking` DISABLE KEYS */;
INSERT INTO `Booking` VALUES (1,'State29','','','1',384),(2,'State29','','','Riya',384);
/*!40000 ALTER TABLE `Booking` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `DOCTOR`
--

DROP TABLE IF EXISTS `DOCTOR`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `DOCTOR` (
  `Department` varchar(255) NOT NULL,
  `Doctor_Id` int NOT NULL AUTO_INCREMENT,
  `Doctor_Name` varchar(255) NOT NULL,
  PRIMARY KEY (`Doctor_Id`)
) ENGINE=InnoDB AUTO_INCREMENT=14 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `DOCTOR`
--

LOCK TABLES `DOCTOR` WRITE;
/*!40000 ALTER TABLE `DOCTOR` DISABLE KEYS */;
INSERT INTO `DOCTOR` VALUES ('Paediatrics',1,'Dr.Smith'),('Paediatrics',2,'Dr.Johnson'),('Cardiology',3,'Dr.Lee'),('Cardiology',4,'Dr.Patel'),('ENT',5,'Dr,Kim'),('ENT',6,'Dr.Davis'),('General Medicine',7,'Dr.Clark'),('General Medicine',8,'Dr.Brown'),('Neurology',9,'Dr.Harris'),('Neurology',10,'Dr.White'),('Oncology',11,'Dr.Brown'),('Dermatology',12,'Dr.Walker'),('Dermatology',13,'Dr.Green');
/*!40000 ALTER TABLE `DOCTOR` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `Departments`
--

DROP TABLE IF EXISTS `Departments`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `Departments` (
  `department_id` int NOT NULL AUTO_INCREMENT,
  `department_name` varchar(255) NOT NULL,
  PRIMARY KEY (`department_id`),
  UNIQUE KEY `department_name` (`department_name`)
) ENGINE=InnoDB AUTO_INCREMENT=21 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `Departments`
--

LOCK TABLES `Departments` WRITE;
/*!40000 ALTER TABLE `Departments` DISABLE KEYS */;
INSERT INTO `Departments` VALUES (1,'Cardiology'),(19,'Dermatology'),(17,'ENT'),(18,'General Medicine'),(2,'Neurology'),(20,'Oncology'),(16,'Paediatrics');
/*!40000 ALTER TABLE `Departments` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `DiagnosticBookings`
--

DROP TABLE IF EXISTS `DiagnosticBookings`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `DiagnosticBookings` (
  `booking_id` int NOT NULL AUTO_INCREMENT,
  `patient_name` varchar(255) NOT NULL,
  `patient_phone` varchar(15) NOT NULL,
  `test_id` int NOT NULL,
  `booking_date` date NOT NULL,
  `booking_time` time NOT NULL,
  `token_number` int DEFAULT NULL,
  PRIMARY KEY (`booking_id`),
  KEY `test_id` (`test_id`),
  CONSTRAINT `DiagnosticBookings_ibfk_1` FOREIGN KEY (`test_id`) REFERENCES `DiagnosticTests` (`test_id`) ON DELETE CASCADE
) ENGINE=InnoDB AUTO_INCREMENT=7 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `DiagnosticBookings`
--

LOCK TABLES `DiagnosticBookings` WRITE;
/*!40000 ALTER TABLE `DiagnosticBookings` DISABLE KEYS */;
INSERT INTO `DiagnosticBookings` VALUES (1,'Hanana','7592079847',4,'2025-06-12','12:33:09',NULL),(2,'Han','9595959595',2,'2026-12-23','13:11:00',NULL),(3,'hae','9087612345',1,'2025-09-12','23:09:00',3),(4,'siya','1234567890',1,'2024-09-12','12:34:45',4),(5,'hiya','9012345609',1,'2024-09-12','12:09:08',5),(6,'dea','1234567890',1,'2025-09-21','12:03:34',6);
/*!40000 ALTER TABLE `DiagnosticBookings` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `DiagnosticTests`
--

DROP TABLE IF EXISTS `DiagnosticTests`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `DiagnosticTests` (
  `test_id` int NOT NULL AUTO_INCREMENT,
  `test_name` varchar(255) NOT NULL,
  `test_description` text,
  PRIMARY KEY (`test_id`)
) ENGINE=InnoDB AUTO_INCREMENT=5 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `DiagnosticTests`
--

LOCK TABLES `DiagnosticTests` WRITE;
/*!40000 ALTER TABLE `DiagnosticTests` DISABLE KEYS */;
INSERT INTO `DiagnosticTests` VALUES (1,'X-Ray','Radiographic imaging to check for fractures and infections'),(2,'CT Scan','Detailed cross-sectional imaging for diagnosing internal injuries'),(3,'MRI Scan','Advanced imaging for soft tissues and organs'),(4,'Blood Test','General health check, including blood count and sugar levels');
/*!40000 ALTER TABLE `DiagnosticTests` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `Doctors`
--

DROP TABLE IF EXISTS `Doctors`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `Doctors` (
  `doctor_id` int NOT NULL AUTO_INCREMENT,
  `doctor_name` varchar(255) NOT NULL,
  `department_id` int NOT NULL,
  PRIMARY KEY (`doctor_id`),
  KEY `department_id` (`department_id`),
  CONSTRAINT `Doctors_ibfk_1` FOREIGN KEY (`department_id`) REFERENCES `Departments` (`department_id`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB AUTO_INCREMENT=22 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `Doctors`
--

LOCK TABLES `Doctors` WRITE;
/*!40000 ALTER TABLE `Doctors` DISABLE KEYS */;
INSERT INTO `Doctors` VALUES (1,'Dr. Alice',2),(2,'Dr. John Doe',1),(3,'Dr. madhu',1),(4,'Dr. Smith',16),(5,'Dr. Johnson',16),(6,'Dr. Lee',1),(7,'Dr. Patel',1),(8,'Dr. Kim',17),(9,'Dr. Davis',17),(10,'Dr. Brown',18),(11,'Dr. Clark',18),(12,'Dr. Harris',2),(13,'Dr. White',2),(14,'Dr. Walker',19),(15,'Dr. Green',19),(16,'Dr. Brown',20);
/*!40000 ALTER TABLE `Doctors` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `PatientBookings`
--

DROP TABLE IF EXISTS `PatientBookings`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `PatientBookings` (
  `booking_id` int NOT NULL AUTO_INCREMENT,
  `patient_name` varchar(255) NOT NULL,
  `patient_phone` varchar(15) NOT NULL,
  `doctor_id` int NOT NULL,
  `booking_date` date NOT NULL,
  `booking_time` time NOT NULL,
  `token_number` int DEFAULT NULL,
  PRIMARY KEY (`booking_id`),
  KEY `doctor_id` (`doctor_id`),
  CONSTRAINT `PatientBookings_ibfk_1` FOREIGN KEY (`doctor_id`) REFERENCES `Doctors` (`doctor_id`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB AUTO_INCREMENT=25 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `PatientBookings`
--

LOCK TABLES `PatientBookings` WRITE;
/*!40000 ALTER TABLE `PatientBookings` DISABLE KEYS */;
INSERT INTO `PatientBookings` VALUES (1,'brown','1234567890',4,'2025-12-12','12:30:00',1000),(2,'renu','1234567890',4,'2025-12-12','12:30:00',1),(3,'renu','1234567890',4,'2025-12-12','12:30:00',2),(4,'renu','1234567890',4,'2025-12-12','12:30:00',3),(5,'renu','1234567890',8,'2025-12-12','12:30:00',1),(6,'renu','1234567890',8,'2025-12-12','12:30:00',2),(7,'giya','1234512345',8,'2023-09-21','09:21:20',3),(8,'Diya','3456709876',14,'2025-12-23','12:09:00',1),(9,'wen','9087612345',14,'2025-12-23','12:34:09',2),(10,'den','2345678906',6,'2025-03-12','12:34:45',1),(11,'fen','1234567890',8,'2025-09-23','23:09:09',4),(12,'fer','9087612345',12,'2025-12-21','12:07:00',1),(13,'diya','1234567890',12,'2025-12-23','12:09:00',2),(14,'hari','1234567890',1,'2025-09-12','12:09:12',1),(15,'dera','8907621345',6,'2025-09-12','12:34:23',2),(16,'dera','8907621345',6,'2025-09-12','12:34:23',3),(17,'dera','8907621345',6,'2025-09-12','12:34:23',4),(18,'dera','8907621345',6,'2025-09-12','12:34:23',5),(19,'dera','8907621345',6,'2025-09-12','12:34:23',6),(20,'dera','8907621345',6,'2025-09-12','12:34:23',7),(21,'Hana','1234567890',6,'2025-09-01','12:12:00',8),(22,'Haaa','1234567890',6,'1221-12-12','12:12:12',9),(23,'HAna','1234567890',6,'2025-12-12','12:12:00',10),(24,'haneena','1234567800',6,'2024-09-09','11:11:11',1);
/*!40000 ALTER TABLE `PatientBookings` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `States`
--

DROP TABLE IF EXISTS `States`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `States` (
  `ID` int NOT NULL AUTO_INCREMENT,
  `Description` varchar(100) DEFAULT NULL,
  `Message` text NOT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=31 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `States`
--

LOCK TABLES `States` WRITE;
/*!40000 ALTER TABLE `States` DISABLE KEYS */;
INSERT INTO `States` VALUES (1,'Welcome','Welcome to Hospital Services! How can I assist you today?'),(2,'Service Selection','Please select a service:\n1) Book a Doctor\n2) Call an Ambulance\n3) Admission Details\n4) Diagnostic Services\n5) Bill and Payment\n6) Insurance Enquiry\n7) Pharmacy'),(3,'Book a Doctor','You selected Book a Doctor.\nWhich department are you looking for?\n1) Paediatrics\n2) Cardiology\n3) ENT\n4) General Medicine\n5) Neurology\n6) Dermatology\n7) Oncology\n\nEnter the number to view available doctors.'),(4,'Call an Ambulance','You selected Call an Ambulance.\nPlease call this number'),(5,'Admission Details','You selected Admission Details.\nAre you looking for inpatient or outpatient admission?.... IP / OP'),(6,'Diagnostic Services','You selected Diagnostic Services.\nWhich test are you looking for?\n1) XRAY\n2) CTSCAN\n3) MRISCAN\n4) BLOODTEST'),(7,'Bill loading....','Here is your bill'),(8,'Insurance Enquiry','You selected Insurance Enquiry.\nDo you have an insurance policy with us?'),(9,'Pharmacy','You selected Pharmacy'),(10,'Select Doctor','Here is the list of available doctors in Paediatrics department. Please choose doctor id and press ok to book\n1)   Dr.  Smith  - 4  -  -  -  -  -  -  - Sun - Tues\n2)   Dr.  Johnson  - 5 -   -  -  -  -  -  -  - Wed - Sat'),(11,'Select Doctor','Here is the list of available doctors in Cardiology department. Please choose doctor id and press ok to book\n1)Dr. Lee  - 6  -  -  -  -  -  -  - Sun - Tues\n2Dr. Patel  - 7 -  -  -  -  -  - Wed - Sat'),(12,'Select Doctor','Here is the list of available doctors in ENT department. Please choose dovtor id and press ok to book\n1) Dr.  Kim  - 8 -     -  -  -  -  -  -  - Sun - Tues\n2Dr.  Davis  - 9 -   -  -  -  -  -  - Wed - Sat'),(13,'Select Doctor','Here is the list of available doctors in General Medicine. Please choose doctor id and press ok to book\n1 )Dr. Brown  - 1 0 -   -  -  -  -  -  -  - Sun - Tues\n2Dr. Clark  - 1 1  -    -  -  -  -  -  -  - Wed - Sat'),(14,'Select Doctor','Here is the list of available doctors in Neurology department. Please choose doctor id an dpress ok to book\n\n1 )Dr. Harris  - 1 2 -   -  -  -  -  -  -  - Sun - Tues\n2)Dr. White  - 1 3 -   -  -  -  -  -  -  - Wed - Sat'),(15,'Select Doctor','Here is the list of available doctors in Dermatalogy department. Please choose doctor id and press ok to book\nn1 )Dr. Walker  - 1 4 -  -  -  -  -  -  -  - Sun - Tues\n2)Dr. Green  - 1 5 -    -  -  -  -  -  -  - Wed - Sat'),(16,'Select Doctor','Here is the list of available doctors in Oncology. Please choose doctor id and press ok to book\n1 )Dr. Brown  - 1 6  -  -  -  -  -  -  - Sun,Tues,Fri'),(18,'NUMBER','9988777443'),(19,'YOU ARE AN INPATIENT','ADMITTED'),(20,'YOU ARE AN OUTPATIENT','PLEASE WAIT'),(21,'Select','1) BOOK 2) RESULT'),(22,'BILL IS','PAY'),(23,'ok','booked'),(24,'Diagnostic Result','Here is your diagnostic result.'),(27,'Query Satisfaction','Are you satisfied with the details provided?'),(28,'End','Thank you for choosing our chatbot. Have a great day!'),(29,'Capture Patient Details','Please provide your details:\n1) Name\n2) Phone Number\n3) Doctor ID\n4) Booking Date (YYYY-MM-DD)\n5) Booking Time (HH:MM:SS)'),(30,'Capture Diagnostic Details','Please provide your details:\n1) Name\n2) Phone Number\n3) Test ID\n4) Booking Date (YYYY-MM-DD)\n5) Booking Time (HH:MM:SS)');
/*!40000 ALTER TABLE `States` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `Transitions`
--

DROP TABLE IF EXISTS `Transitions`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `Transitions` (
  `FromState` int NOT NULL,
  `Answer` varchar(100) NOT NULL,
  `ToState` int NOT NULL,
  PRIMARY KEY (`FromState`,`Answer`),
  KEY `ToState` (`ToState`),
  CONSTRAINT `Transitions_ibfk_1` FOREIGN KEY (`FromState`) REFERENCES `States` (`ID`) ON DELETE CASCADE,
  CONSTRAINT `Transitions_ibfk_2` FOREIGN KEY (`ToState`) REFERENCES `States` (`ID`) ON DELETE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `Transitions`
--

LOCK TABLES `Transitions` WRITE;
/*!40000 ALTER TABLE `Transitions` DISABLE KEYS */;
INSERT INTO `Transitions` VALUES (1,'Service Selection',2),(27,'no',2),(2,'1',3),(2,'book',3),(2,'book a doctor',3),(2,'doctor',3),(2,'2',4),(2,'ambulance',4),(2,'call',4),(2,'call an ambulance',4),(2,'3',5),(2,'admission',5),(2,'4',6),(2,'diagnose',6),(2,'diagnostic',6),(2,'diagnostic services',6),(2,'5',7),(2,'bill',7),(2,'pay',7),(2,'payment',7),(2,'6',8),(2,'insurance',8),(2,'7',9),(2,'medicine',9),(2,'pharmacy',9),(3,'1',10),(3,'paediatrics',10),(3,'2',11),(3,'cardiology',11),(3,'heart',11),(3,'3',12),(3,'ent',12),(3,'4',13),(3,'general',13),(3,'5',14),(3,'neurology',14),(3,'6',15),(3,'dermatology',15),(3,'general medicine',15),(3,'7',16),(3,'oncology',16),(4,'ok',18),(5,'i p',19),(5,'ip',19),(5,'o p',20),(5,'op',20),(6,'1',21),(6,'2',21),(6,'3',21),(6,'4',21),(6,'blood',21),(6,'bloodtest',21),(6,'c t',21),(6,'c t scan',21),(6,'m r i',21),(6,'m r i scan',21),(6,'xray',21),(7,'ok',22),(21,'2',24),(27,'yes',28),(10,'ok',29),(11,'ok',29),(12,'ok',29),(13,'ok',29),(14,'ok',29),(15,'ok',29),(16,'ok',29),(21,'1',30),(21,'book',30);
/*!40000 ALTER TABLE `Transitions` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `Transitions_backup`
--

DROP TABLE IF EXISTS `Transitions_backup`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `Transitions_backup` (
  `FromState` int NOT NULL,
  `Answer` varchar(100) NOT NULL,
  `ToState` int NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `Transitions_backup`
--

LOCK TABLES `Transitions_backup` WRITE;
/*!40000 ALTER TABLE `Transitions_backup` DISABLE KEYS */;
INSERT INTO `Transitions_backup` VALUES (1,'Service Selection',2),(2,'1',3),(2,'2',4),(2,'3',5),(2,'4',6),(2,'5',7),(2,'6',8),(2,'7',9),(2,'8',10),(2,'9',11),(3,'1',12),(3,'2',13),(3,'3',14),(3,'4',15),(3,'5',16),(3,'6',17),(3,'7',18),(6,'1',20);
/*!40000 ALTER TABLE `Transitions_backup` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `diagnostic_token_counter`
--

DROP TABLE IF EXISTS `diagnostic_token_counter`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `diagnostic_token_counter` (
  `test_id` int NOT NULL,
  `current_token` int NOT NULL DEFAULT '1',
  PRIMARY KEY (`test_id`),
  CONSTRAINT `fk_test_id_token` FOREIGN KEY (`test_id`) REFERENCES `DiagnosticTests` (`test_id`) ON DELETE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `diagnostic_token_counter`
--

LOCK TABLES `diagnostic_token_counter` WRITE;
/*!40000 ALTER TABLE `diagnostic_token_counter` DISABLE KEYS */;
INSERT INTO `diagnostic_token_counter` VALUES (1,7);
/*!40000 ALTER TABLE `diagnostic_token_counter` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `token_counter`
--

DROP TABLE IF EXISTS `token_counter`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `token_counter` (
  `doctor_id` int NOT NULL,
  `current_token` int NOT NULL DEFAULT '1',
  PRIMARY KEY (`doctor_id`),
  CONSTRAINT `token_counter_ibfk_1` FOREIGN KEY (`doctor_id`) REFERENCES `Doctors` (`doctor_id`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `token_counter`
--

LOCK TABLES `token_counter` WRITE;
/*!40000 ALTER TABLE `token_counter` DISABLE KEYS */;
INSERT INTO `token_counter` VALUES (6,2);
/*!40000 ALTER TABLE `token_counter` ENABLE KEYS */;
UNLOCK TABLES;
/*!40103 SET TIME_ZONE=@OLD_TIME_ZONE */;

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;

-- Dump completed on 2025-06-29 10:49:55
