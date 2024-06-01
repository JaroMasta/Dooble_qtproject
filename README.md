# Gra

**Gra polega na tym, że ustawiamy jedną grupę kart na stole, drugą rozdzielamy między graczami.** Jeśli gracz znajdzie dopasowanie ze swoją kartą i kartą ze wspólnego stosu, zabiera obie. Gracz może stwierdzić, że nie ma dopasowania, wówczas inni gracze sprawdzają, czy ma rację. Jeśli tak, zabiera kartę, a jeśli nie, traci życie. **Dla każdej karty ze zbioru A, dla każdej karty ze zbioru B istnieje maksymalnie 1 dopasowanie!**

**Zalecam korzystanie z 10 do 15 elementów i kart 4-elementowych. Wówczas algorytm działa najlepiej.**

## Jak korzystać z programu?

1. **Najpierw należy nacisnąć rozpocznij.**
2. **Wybierz wymiary obrazów** (tyczy się to ich wszystkich, nie powinieneś później tego zmieniać)!
3. **Dodaj obrazy tak, aby numery po prawej i po lewej się zgadzały**. Możesz przesuwać i usuwać obrazy, aby dostosować numerację.
4. **Wybierz suwakiem, ile chcesz elementów na jednej karcie** (polecam 4, 6 lub 9, jeśli mamy czas poczekać).
5. **Zapisz obraz**. Jeśli jest pusty, oznacza to, że nie istnieje żadna konfiguracja przy zadanych warunkach. Spróbuj dodać więcej obrazków lub zmniejszyć liczbę obrazków na jednej karcie.

## TO DO

- W grze można za pomocą niewielkiej zmiany kodu lub dodania kodu stworzyć różne tryby. Na przykład **jedna grupa kart może być znacznie liczniejsza niż druga.**
- Można także **usunąć karty, które nie mają dopasowania dla każdej karty z grupy przeciwnej.**
- Można przyspieszyć działanie algorytmu, **wykorzystując inne struktury danych** niż QVector (QMap, QSet, QHash).
- Można dodać **opcję dodawania tekstu zamiast obrazów.**
