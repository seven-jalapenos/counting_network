
import subprocess
import os

path_to_python = 'python'
benchmark_helper = '/home/jjurgenson/code_stuff/CombiningCounterTest/test/benchmark_helper.py'
bench = '/home/jjurgenson/code_stuff/counting_network/test/bench.py'
counting_bm = '/home/jjurgenson/code_stuff/counting_network/test/bin/counting_bm'

def main():
	subprocess.run([path_to_python, benchmark_helper, 'prepare', '--cores', '0-11'])

	subprocess.run([
		'systemd-run',
		'--scope',
		'--slice=bench.slice',
		path_to_python, bench
	])
	env = os.environ.copy()
	subprocess.run([
		'systemd-run',
		'--scope',
		'--slice=bench.slice',
		counting_bm
	], env=env)
	
	subprocess.run([path_to_python, benchmark_helper, 'restore'])

if __name__ == '__main__':
	main()