SELECT Zdarzenia.IDZdarzenia AS [Numer Zdarzenia], Zdarzenia.NazwaZdarzenia AS [Nazwa], Zdarzenia.CzasZdarzenia AS [Czas], Typ.TypZdarzenia AS [Typ], Miejsce.Wspolrzedne AS [Wspó³rzêdne], Uzytkownik.Login AS [Zg³aszaj¹cy] 
FROM Zdarzenia 
INNER JOIN Typ ON Zdarzenia.IDTypu=Typ.IDTypu 
INNER JOIN Miejsce ON Zdarzenia.IDMiejsca=Miejsce.IDMiejsca 
inner JOIN Uzytkownik ON Zdarzenia.IDUzytkownika=Uzytkownik.IDUzytkownika