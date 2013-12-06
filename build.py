import ConfigParser
import getopt
from os import path
import os
import re
import subprocess
from subprocess import Popen
import sys
import urllib

# functions

def Usage():
	print ''
	print 'build.py [-h][-f] [-p=(Project|Tests|Dependencies)] [--tests]'
	print ''
	print '-h or --help'
	print '\t\tDisplay this message'
	print '-p or --project'
	print '\t\tSpecify a project to build'
	print '-f or --fast'
	print '\t\tBuild only the release target'
	print '--debug'
	print '\t\tDebug mode'
	print '--tests'
	print '\t\tBuild and run all tests'
	print '--all'
	print '\t\tBuild and run dependencies, project and tests'
	print ''

def PauseAndExit(code):
	raw_input("Press ENTER to exit.")
	sys.exit(code)	

# get compiler

def parseEnv(envoutput):
	handle_line = lambda l: tuple(l.rstrip().split("=", 1))
	pairs = map(handle_line, envoutput)
	valid_pairs = filter(lambda x: len(x) == 2, pairs)
	valid_pairs = [(x[0].upper(), x[1]) for x in valid_pairs]
	return dict(valid_pairs)

def overrideEnv(newenv):
	old = os.environ.copy()
	removed = set(old) - set(newenv)
	for k in newenv.keys():
		os.environ[k] = newenv[k]
	for k in removed:
		os.environ.pop(k)
	return old

def setupVSEnv(vsver):
	si = subprocess.STARTUPINFO()
	si.dwFlags |= subprocess._subprocess.STARTF_USESHOWWINDOW
	cmd = r'cmd /s /c ""%VS{vsver}COMNTOOLS%vsvars32.bat" & set"'.format(**locals())
	ret = Popen(
		cmd,
		startupinfo=si,
		stdout=subprocess.PIPE,
		stderr=subprocess.STDOUT,
		stdin=subprocess.PIPE
	)
	output = ret.communicate()[0]
	output = output.split("\r\n")
	old = overrideEnv(parseEnv(output))
	return old

setupVSEnv(100)

cpp_compiler = os.environ['FrameworkDir'] + os.environ['FrameworkVersion'] + "\\msbuild.exe"
	
# options

projects = []
fast = False
debug = False

# check command line arguments

if len(sys.argv) == 1:
	projects.append('Dependencies')
	projects.append('Project')
	projects.append('Tests')
else:
	try:
		command_line = sys.argv[1:]
		options, arguments = getopt.getopt(
			command_line,
			"hp:f",
			['help', 'project=', 'fast', 'tests', 'debug', 'all']
		)
	except getopt.GetoptError:
		Usage()
		sys.exit(2)
		
	for option, argument in options:
		if option in ('-h', '--help'):
			Usage()
			sys.exit()
		elif option in ('-p', '--project'):
			projects = argument.split(';')
		elif option in ('-f', '--fast'):
			fast = True
		elif option == '--debug':
			debug = True
		elif option == '--all':
			projects.append('Dependencies')
			projects.append('Project')
			projects.append('Tests')
		elif option == '--tests':
			projects.append('Tests')
			fast = True
		
# build

class Solution:
	def __init__(self, solution):
		self.solution = solution
		self.projects = []
		self.targets = []

	def build(self, project, target, platform):
		flags = '/nologo'
		
		if project:
			safe_project = urllib.quote(project.replace('.', '_'))
			flags += ' /t:\"' + safe_project + '\"'
		
		if target:
			flags += ' /p:Configuration=' + target;
			
		if platform:
			flags += ' /p:Platform=' + platform;
		
		success = True
		
		try:
			command = '\"' + cpp_compiler + '\" \"' + self.solution + '\" ' + flags
			if debug:
				print command
				print ''
				
			output = subprocess.check_call(command)
			if not output == 0:
				success = False
		except subprocess.CalledProcessError:
			success = False
			print ''
			print '--- Failed to compile project.'
			
		return success
		
	def build_list(self):
		print self.solution
		print self.projects
					
		for target in self.targets:
			if len(self.projects) > 0:
				for project in self.projects:
					if not self.build(project, target, 'Win32'):
						print ''
						print '--- Failed to build \"' + project + '\" on ' + target + '.'
						print ''
						return False
			else:
				if not self.build(None, target, 'Win32'):
					print ''
					print '--- Failed to build \"' + project + '\" on ' + target + '.'
					print ''
					return False
						
		return True

solutions = []
tests = []

if 'Dependencies' in projects:
	s = Solution('ExLibrisGL.Dependencies.sln')
	solutions.append(s)
	
if 'Project' in projects or 'Tests' in projects:
	s = Solution('ExLibrisGL.sln')

	if 'Project' in projects:
		print '--- Building project'
		print ''
		
		s.projects.append('ExLibrisGL')
		s.projects.append('Example')
	elif 'Tests' in projects:
		print '--- Building tests'
		print ''
		
		s.projects.append('ExLibris.Test')
		
		if fast:
			tests.append('ExLibris.TestRelease.exe')
		else:
			tests.append('ExLibris.TestDebug.exe')
			tests.append('ExLibris.TestRelease.exe')
			
	solutions.append(s)
	
# build projects
	
if len(solutions) > 0:
	print '--- Executing'
	print ''
	
	if debug:
		for s in solutions:
			print s.solution
			print s.projects
			print ''
			
	for s in solutions:
		if fast:
			s.targets.append('Release')
		else:
			s.targets.append('Debug')
			s.targets.append('Release')
		
		if not s.build_list():
			break
		
		print ''
else:
	Usage()
	sys.exit(2)

# run tests

if len(tests) > 0:
	print '--- Running tests'
	
	for t in tests:
		try:
			print ''
			
			command = 'pushd Build && call \"' + t + '\" && popd'
			if debug:
				print command
				
			subprocess.call(command, shell=True)
			print ''
		except subprocess.CalledProcessError:
			print ''
			print 'Could not run tests.'
		
PauseAndExit(0)