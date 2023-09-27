import matplotlib.pyplot as plt


def show_graphs():
    time = list()
    threads_count = list()

    with open("data.txt", "r") as file:
        list_of_data = file.readlines()
        for i in range(len(list_of_data)):
            if list_of_data[i][len(list_of_data[i]) - 1] == "\n" or list_of_data[i][len(list_of_data[i]) - 1] == " ":
                list_of_data[i] = list_of_data[i][0:len(list_of_data[i]) - 1].strip()

        
        for str in list_of_data:
            str = str.split(" ")
            if ("threads_count" in str[0]):
                threads_count.append(int(str[1]))
            else:
                time.append(float(str[1]))
                
        plt.grid(True)
        plt.title("Зависимость времени выполнения от числа потоков")
        plt.xlabel("Число потоков")
        plt.ylabel("Время выполнения")
        plt.plot(threads_count, time, '-*')
        plt.legend()
        plt.show()    
        
            
show_graphs()
