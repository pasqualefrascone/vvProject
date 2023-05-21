def stampa_numeri():
    with open("numeri.txt", "w") as file:
        for i in range(1, 2001):
            if i % 2 == 0:
                numero = 1000 - int((i/2))
            else:
                numero = 1000
            file.write(str(numero) + "\n")

if __name__ == "__main__":
    stampa_numeri()