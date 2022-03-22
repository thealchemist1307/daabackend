from subprocess import Popen, PIPE, call, check_output
import os

def main():
    data, temp = os.pipe()
    os.write(temp, bytes("6\n1 1 11 11\n1 11 11 1\n1 11 11 11\n11 11 11 1\n11 1 1 1\n1 1 1 11", "utf-8"))
    os.close(temp)
    s = check_output(["sweep.exe"], stdin=data, shell=True)
    print(s.decode("utf-8"))
    #tmp.stdin.write(str)
    #print(tmp.stdout.read())
    
if __name__ == "__main__":
    main()
