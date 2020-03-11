import matplotlib.pyplot as plt


def plot_file_relationship(file_name, operation):
    file = open(file_name, "r")
    time_data = []
    while True:
        line = file.readline()
        if not line:
            break
        time_data.append(1*float(line))
    file.close()
    n = len(time_data)
    plt.title("Typical relationship between number of elements in SPL tree and time" + \
              "\n for operation " + operation)
    plt.plot(range(n), time_data[:], '.:', linewidth=0.1, markersize=0.7)
    plt.xlabel("number of elements")
    plt.ylabel("time (microseconds)")
    plt.savefig(operation + ".png")
    plt.show()


plot_file_relationship("study_insert_output.txt", "insert")
plot_file_relationship("study_search_output.txt", "search")
plot_file_relationship("study_delete_output.txt", "delete")