import json

#task number
task_num = 2

#number of nodes in the tasks
number_of_nodes = [7, 5]

#set offsets
task_offset = 5
sequence_offset = 9
node_ofset = 5
job_offset = 4
time_offset = 7
###################################

WCRT_list = [0,0]
event_list = []
executions_list = []
cpu_assignment = [[1,2,3,4], [1,2,3]]

class RBS_execution:
    def __init__(self, task, sequence, node, job, start, end, executionTime):
        self.task = task
        self.sequence = sequence
        self.node = node
        self.job = job
        self.start = start
        self.end = end
        self.executionTime = executionTime
        self.responseTime = 0
        self.releaseTime = 0


class RBS_event:
    def __init__(self, type, task, sequence, node, job, start):
        self.type = type
        self.task = task
        self.sequence = sequence
        self.node = node
        self.job = job
        self.start = start
        self.end = 0
        self.duration = 0
        self.cpu = 0


def generate_trace():

    with open("sample.json", "w") as outfile: 

        dictionaries = []  
        for element in event_list:

            if element.type == 1:

                name_string = "TASK " + str(element.task) + ", NODE " + str(element.node) + ", JOB " + str(element.job)


                if task_num > 4:
                    # Data to be written
                    dictionary = {
                        "pid": element.cpu,
                        "tid": element.cpu,
                        "ts": element.start,
                        "dur": element.duration,
                        "ph": "X",
                        "name": name_string
                    }

                else:

                    color = ""

                    if element.task == 1:
                        color = "yellow"
                    elif element.task == 2:
                        color = "grey"
                    elif element.task == 3:
                        color = "white"
                    else:
                        color = "black"

                    # Data to be written
                    dictionary = {
                        "pid": element.cpu,
                        "tid": element.cpu,
                        "ts": element.start,
                        "dur": element.duration,
                        "ph": "X",
                        "name": name_string,
                        "cname" : str(color)
                    }

                dictionaries.append(dictionary)

            elif element.type == 2:
                name_string = "RELEASE OF TASK " + str(element.task)  + ", JOB " + str(element.job)

                dictionary = {
                    "pid": 1,
                    "tid": 1,
                    "ts": element.start,
                    "ph": "i",
                    "name": name_string,
                    "s": "g"
                }

                dictionaries.append(dictionary)

            else:
                continue

        json.dump(dictionaries, outfile)
        



def solve_preemptions():
    for element in event_list:
        if element.task == 1 or element.type != 1:
            continue
        else:
            for element2 in event_list:
                if element2.task < element.task:
                    if element.cpu == element2.cpu:
                        if (element2.start > element.start and element2.end < element.end and element2.end < element.end):

                            new_event = RBS_event(1, element.task, element.sequence, element.node, element.job, element2.end)
                            new_event.end = element.end
                            new_event.cpu = element.cpu

                            element.end = element2.start

                            event_list.append(new_event)


def transformEventsToExecutions():
    counter = 0
    for event in event_list:
        if event.type != 1:
            continue
        for element in executions_list:
            if element.task == event.task and element.node == event.node and element.job == event.job:

                #Detected slice of node was executed later than existing one
                if (event.end > element.end) and (event.start > element.start):
                    element.executionTime = element.executionTime + event.duration
                    element.end = event.end

                #Detected slice of node was executed earlier than existing one
                if (event.end < element.end) and (event.start < element.start):
                    element.executionTime = element.executionTime + event.duration
                    element.start = event.start

        new_execution = RBS_execution(event.task, event.sequence, event.node, event.job, event.start, event.end, event.duration)
        executions_list.append(new_execution)
    return


def compute_WCRT():
    for event in event_list:
        if event.type == 2:
            for execution in executions_list:
                if execution.task == event.task and execution.job == event.job and execution.node == number_of_nodes[(execution.task-1)]:
                    wc_response_t = execution.end - event.start
                    if wc_response_t > WCRT_list[(execution.task-1)]:
                        WCRT_list[(execution.task-1)] = wc_response_t

def compute_WCET():
    return



def add_cpu():
    for event in event_list:
        task_index = event.task -1
        sequence_index = event.sequence -1
        cpu = cpu_assignment[task_index][ sequence_index]
        event.cpu = cpu


def compute_duration():
    for event in event_list:
        event.duration = event.end - event.start

def obtain_value(line_string, value_type, offset, end_mark):
    index = line_string.find(value_type)
    extra_offset = 1
    while True:
        temp_index = index + offset + extra_offset
        if line_string[temp_index] == end_mark:
            extra_offset = extra_offset - 1
            break
        else:
            extra_offset = extra_offset + 1

    if(extra_offset == 0):
        return_value = line_string[index + offset]
    else:
        return_value = line_string[index + offset :(index + offset+ extra_offset + 1)]

    return int(return_value)



 
def read_and_convert_log():
    with open("log.txt", "r") as log:
        for line in log:
            line_string = str(line)
            result = line_string.find("NODE_EXECUTION_STARTED")
            if result != (-1):

                #find task number
                task = obtain_value(line_string,'task',task_offset,',')

                #find sequence number
                sequence = obtain_value(line_string,'sequence',sequence_offset,',')

                #find node number
                node = obtain_value(line_string,'node',node_ofset,',')

                #find job number
                job = obtain_value(line_string,'job',job_offset,',')

                #find start time
                start = obtain_value(line_string,'cycle', time_offset,'.')

                #create event
                event = RBS_event(1, task, sequence, node, job, start)
                event_list.append(event)

                continue

            result = line_string.find("NODE_EXECUTION_FINISHED")
            if result != (-1):

                #find task number
                task = obtain_value(line_string,'task', task_offset,',')

                #find sequence number
                sequence = obtain_value(line_string,'sequence', sequence_offset,',')

                #find node number
                node = obtain_value(line_string,'node', node_ofset,',')

                #find job number
                job = obtain_value(line_string,'job', job_offset,',')

                #find end time
                end = obtain_value(line_string,'cycle', time_offset,'.')

                for element in event_list:
                    if (element.task == task and element.sequence == sequence and element.node == node and element.job == job):
                        element.end = end
                continue

            result = line_string.find("NEW_JOB_RELEASED")
            if result != (-1):

                #find task number
                task = obtain_value(line_string,'task',task_offset,',')

                #find job number
                job = obtain_value(line_string,'job',job_offset,',')

                #find start time
                start = obtain_value(line_string,'cycle', time_offset,'.')

                #create event
                event = RBS_event(2, task, 0, 0, job, start)
                event_list.append(event)

                continue
        

def main():

    read_and_convert_log()
    add_cpu()
    solve_preemptions()
    compute_duration()
    generate_trace()
    transformEventsToExecutions()
    compute_WCRT()
    print("WCRTs: ", WCRT_list)

    #for element in event_list:
        #print("task =", element.task,"sequence = ", element.sequence,"job = ", element.job,"start =", element.start,"end =", element.end,"duration =", element.duration, "cpu =", element.cpu)

    



if __name__ == "__main__":
    main()

