SELECT Zdarzenia.IDZdarzenia, Zdarzenia.NazwaZdarzenia, Zdarzenia.CzasZdarzenia, Typ.TypZdarzenia, Miejsce.Wspolrzedne, Uzytkownik.Login
FROM Zdarzenia 
INNER JOIN Typ ON Zdarzenia.IDZdarzenia=Typ.IDTypu
INNER JOIN Miejsce ON Zdarzenia.IDZdarzenia=Miejsce.IDMiejsca
INNER JOIN Uzytkownik ON Zdarzenia.IDZdarzenia=Uzytkownik.IDUzytkownika