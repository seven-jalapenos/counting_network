
import subprocess
import csv
from itertools import product

EXECUTABLE = "./bin/atomic_op"
OUTPUT_FILE = "atq_results.csv"

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

    extract = lambda s: s.split(' ')[-1:]

    with open(OUTPUT_FILE, "w", newline="") as csvfile:
        writer = csv.writer(csvfile)

        writer.writerow(["threads", "megaops/sec"])

        for thread in threads:
            results = [thread]
            args = [thread]
            res = (extract(run_once(args)))
            results.extend(res)
            print(f"Ran {args} -> {res}")
            writer.writerow(results)


if __name__ == "__main__":
    main()