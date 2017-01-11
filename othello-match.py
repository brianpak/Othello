import subprocess
from time import sleep
import os

def main():
    f = "othello_match"
    
    for i in range(1, 6):
        os.remove(f+'s_d'+str(i)) if os.path.exists(f+'s_d'+str(i)) else None
        with open(f+'s_d'+str(i), 'a') as outfile:
            for j in range(10):
                args="./othello-serial -d %d -t 1" % i
                output=subprocess.Popen(args, shell=True, stdout=subprocess.PIPE).stdout.read()
                outfile.write(output)
                sleep(1)

    # thread=["1","2","4","8","16"]
    # for t in thread:
    #     os.remove(f+'_d5_t'+t) if os.path.exists(f+'_d5_t'+t) else None
    #     with open(f+'_d5_t'+t, 'a') as outfile:
    #         for j in range(10):
    #             args="./othello-parallel -d 5 -t %s" % t
    #             output=subprocess.Popen(args, shell=True, stdout=subprocess.PIPE).stdout.read()
    #             outfile.write(output)
    #             sleep(1)

if __name__=="__main__":
    main()
