class Dog:
    species = 'caniche'

    def __init__(self, name, age):
        self.name = name
        self.age = age

bambi = Dog("Bambi", 8)
mikey = Dog("Rufus", 6)

print(("{} is {} and {} is {}.").format(bambi.name, bambi.age, mikey.name, mikey.age))
def get_biggest_number(*argumentos):
    numArgumentos = len(argumentos)
    if numArgumentos > 0 :
        mayorEdad = -1
        for elemento in argumentos :
            if(elemento > mayorEdad):
                mayorEdad = elemento
        return mayorEdad
    else:
        return 0

gos1 = Dog("Kiwi", 7)
gos2 = Dog("Rex", 1)
gos3 = Dog("Gipsy", 4)

print('La edad mas alta de los perros es::')
print(str(get_biggest_number(gos1.age, gos2.age, gos3.age)))


