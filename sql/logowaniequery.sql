SELECT Uzytkownik.IDUzytkownika AS [Numer Uzytkownika], Uzytkownik.Login AS [Login], Uzytkownik.Haslo AS [Haslo], RodzajUzytkownika.Nazwa AS [Rodzaj Użytkownika] 
FROM Uzytkownik
INNER JOIN RodzajUzytkownika ON Uzytkownik.IDUzytkownika=RodzajUzytkownika.IDRodzaju
WHERE Login = 'marcsien' AND Haslo = 'admin1234'