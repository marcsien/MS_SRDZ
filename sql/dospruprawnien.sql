SELECT Uzytkownik.IDUzytkownika AS [Numer Uzytkownika], Uzytkownik.Login AS [Login], Uzytkownik.Haslo AS [Haslo], RodzajUzytkownika.Nazwa AS [Rodzaj U¿ytkownika], Uprawnienia.IDUprawnienia AS [IDUprawnienia], Uprawnienie.Edycja AS [Edycja], Uprawnienie.Podglad AS [Podglad], Uprawnienie.Dodawanie AS [Dodawanie]
FROM Uzytkownik
INNER JOIN RodzajUzytkownika ON Uzytkownik.IDUzytkownika=RodzajUzytkownika.IDRodzaju
INNER JOIN Uprawnienia ON  Uprawnienia.IDRodzaju=RodzajUzytkownika.IDRodzaju
INNER JOIN Uprawnienie ON  Uprawnienie.IDUprawnienia=Uprawnienia.IDUprawnienia
WHERE Login = 'marcsien' AND Haslo = 'admin1234' AND Edycja = '1'
