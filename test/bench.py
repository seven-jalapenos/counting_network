
import subprocess
import csv
from itertools import product

EXECUTABLE = "./bin/test_queue"
OUTPUT_FILE = "even_op_results_net.csv"
# OUTPUT_FILE_2 = "even_op_results_boost.csv"

widths = ['4', '8', '16']

def run_once(args)->str:
    completed = subprocess.run(
        [EXECUTABLE] + args,
        capture_output=True,
        text=True,
        check=True
    )
    return completed.stdout.strip()


def main():
    threads = [str(x) for x in range(1, 33)]
    widths = ['4', '8', '16']
    ttq = '0'
    # boost = '1'
    # list of argument lists sorted by widths

    extract = lambda s: s.split()[-1:]
    # extract2 = lambda s: s.split()[-2:]

    with open(OUTPUT_FILE, "w", newline="") as csvfile:
        writer = csv.writer(csvfile)

        writer.writerow(["threads", "width=4", "width=8", "width=16"])

        for thread in threads:
            results = [thread]
            for width in widths:
                args = [thread, width, ttq]
                res = (extract(run_once(args)))
                results.extend(res)
                print(f"Ran {args} -> {res}")
            writer.writerow(results)

    # with open(OUTPUT_FILE_2, "w", newline="") as csvfile:
    #     writer = csv.writer(csvfile)

    #     writer.writerow(["threads", "megaops/sec"])

    #     for thread in threads:
    #         results = [thread]
    #         args = [thread, '4', boost]
    #         res = (extract(run_once(args)))
    #         results.extend(res)
    #         print(f"Ran {args} -> {res}")
    #         writer.writerow(results)


if __name__ == "__main__":
    main()
