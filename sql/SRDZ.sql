use master;
drop database SRDZ;
create database SRDZ;
use SRDZ;
go
	
	create table Zdarzenia
	(IDZdarzenia int not null primary key identity(1,1),
	NazwaZdarzenia nvarchar(20),
	IDTypu int not null,
	CzasZdarzenia datetime,
	IDMiejsca int,
	IDUzytkownika int
	);
	
	create table Typ
	(IDTypu int not null primary key identity(1,1),
	TypZdarzenia nvarchar(20),
	);
	
	create table Miejsce
	(IDMiejsca int not null primary key identity(1,1),
	Wspolrzedne nvarchar(50),
	);
	
	create table Uprawnienie
	(IDUprawnienia int not null primary key identity(1,1),
	Nazwa nvarchar(20),
	Edycja bit,
	Podglad bit,
	Dodawanie bit
	);
	
	create table Uprawnienia
	(IDUprawnien int not null primary key identity(1,1),
	IDRodzaju int,
	IDUprawnienia int
	);
	
	create table RodzajUzytkownika
	(IDRodzaju int not null primary key identity(1,1),
	Nazwa nvarchar(20),
	);
	
	create table Uzytkownik
	(IDUzytkownika int not null primary key identity(1,1),
	Imie nvarchar(20),
	Nazwisko nvarchar(20),
	Email nvarchar(40),
	Login nvarchar(20),
	Haslo nvarchar(20),
	IDRodzaju int
	);
	
	create table Obsluga
	(IDObslugi int not null primary key identity(1,1),
	IDZdarzenia int,
	IDUzytkownika int,
	CzasObslugi datetime,
	Komentarz nvarchar(50)
	);
	
	ALTER TABLE Obsluga ADD CONSTRAINT FKObslugaZdarzenia
	FOREIGN KEY (IDZdarzenia) REFERENCES Zdarzenia(IDZdarzenia)
	
	ALTER TABLE Obsluga ADD CONSTRAINT FKObslugaUzytkownik
	FOREIGN KEY (IDUzytkownika) REFERENCES Uzytkownik(IDUzytkownika)
	
	ALTER TABLE Uprawnienia ADD CONSTRAINT FKUprawnieniaUprawnienie
	FOREIGN KEY (IDUprawnienia) REFERENCES Uprawnienie(IDUprawnienia)
	
	ALTER TABLE Uprawnienia ADD CONSTRAINT FKUprawnieniaRodzajUzytkownika
	FOREIGN KEY (IDRodzaju) REFERENCES RodzajUzytkownika(IDRodzaju)
	
	ALTER TABLE Uzytkownik ADD CONSTRAINT FKUzytkownikRodzajUzytkownika
	FOREIGN KEY (IDRodzaju) REFERENCES RodzajUzytkownika(IDRodzaju)
	
	ALTER TABLE Zdarzenia ADD CONSTRAINT FKZdarzeniaUzytkownik
	FOREIGN KEY (IDUzytkownika) REFERENCES Uzytkownik(IDUzytkownika)
	
	ALTER TABLE Zdarzenia ADD CONSTRAINT FKZdarzeniaTyp
	FOREIGN KEY (IDTypu) REFERENCES Typ(IDTypu)
	
	ALTER TABLE Zdarzenia ADD CONSTRAINT FKZdarzeniaMiejsce
	FOREIGN KEY (IDMiejsca) REFERENCES Miejsce(IDMiejsca)
	
	insert into RodzajUzytkownika(Nazwa)
	values 
		('Administrator' ),
		('Dyspozytor' ),
		('Uzytkownik' );
	
	insert into Typ(TypZdarzenia)
	values 
		('Pozar' ),
		('Napad' ),
		('Powodz' ),
		('Obalone_drzewo' ),
		('Wypadek_samochodowy' );	
		
	insert into Uprawnienie(Nazwa , Edycja , Podglad , Dodawanie)
	values 
		('Uzytkowanie' , 0 , 0 , 1 ),
		('Dysponowanie' , 0 , 1 , 1 ),
		('Administrowanie' , 1 , 1 , 1 );
		
	insert into Uprawnienia(IDRodzaju , IDUprawnienia)
	values 
		( 1 , 3 ),
		( 2 , 2 ),
		( 3 , 1 );
	
	insert into Uzytkownik(Imie, Nazwisko, Email, Login, Haslo, IDRodzaju)
	values 
		('Marcin' , 'Sienicki' , 'marcin.sienicki@student.wat.edu.pl' , 'marcsien' , 'admin1234' , 1 ),
		('Admin' , 'Adminowski' , 'admin@admin.pl' , 'admin' , 'admin' , 1 ),
		('Daniel' , 'Kowalski' , 'danielek123@gmail.com' , 'dani' , 'dan123' , 2 ),
		('Jan' , 'Ozdoba' , 'janeczeko@wp.pl' , 'janko' , '998998' , 2 ),
		('Mateusz' , 'Komosa' , 'matika@op.pl' , 'mati' , 'mateo' , 3 ),
		('Maciej' , 'Grzegorek' , 'grzegor@tempmail.com' , 'greg' , 'gregory' , 3 );
		
	insert into Miejsce(Wspolrzedne)
	values 
		( '23.46758 S, 151.02789 E, 18.1m' ),
		( '24.46758 S, 152.02789 E, 25.7m' ),
		( '21.56758 S, 153.02789 E, 14.6m' ),
		( '22.46758 S, 154.02789 E, 18.9m' ),
		( '21.46758 S, 155.02789 E, 17.3m' ),
		( '20.23454 S, 143.23424 E, 12.3m' ),
		( '22.23211 S, 152.54243 E, 19.3m' );
		
	insert into Zdarzenia(NazwaZdarzenia, IDTypu, CzasZdarzenia, IDMiejsca, IDUzytkownika)
	values 
		('jeden' , '1' , '2018-03-11 09:45:11' , 1 , 5 ),
		('dwa' , '2' , '2018-06-12 10:30:14' , 2 , 6 ),
		('trzy' , '3' , '2018-07-13 08:20:15' , 3 , 5 ),
		('cztery' , '4' , '2018-09-14 11:15:45' , 4 , 6 ),
		('piec' , '5' , '2018-11-15 12:10:02' , 5 , 5 ),
		('szesc' , '2' , '2019-01-14 10:25:45' , 6 , 6 ),
		('siedem' , '5' , '2019-05-15 15:15:02' , 7 , 5 );
		
		
	insert into Obsluga(IDZdarzenia, IDUzytkownika, CzasObslugi, Komentarz)
	values 
		(1 , 2 , '2018-03-11 09:50:11' , 'decyzja jeden'),
		(2 , 3 , '2018-06-12 10:36:14' , 'decyzja dwa'),
		(3 , 2 , '2018-07-13 08:27:15' , 'decyzja trzy');