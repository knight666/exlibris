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
	print 'build.py [-h] [-p=(Project|Tests|Dependencies)] [-f]'
	print ''
	print '-h or --help'
	print '\t\tDisplay this message'
	print '-p or --project'
	print '\t\tSpecify a project to build'
	print '-f or --fast'
	print '\t\tBuild only the release target'
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
	
# check command line arguments

try:
	command_line = sys.argv[1:]
	options, arguments = getopt.getopt(
		command_line,
		"hp:f",
		['help', '--project', '--fast']
	)
except getopt.GetoptError:
	Usage()
	PauseAndExit(2)
	
projects = []
fast = False

for option, argument in options:
	if option in ('-h', '--help'):
		Usage()
		sys.exit()
	elif option in ('-p', '--project'):
		projects = argument.split(';')
	elif option in ('-f', '--fast'):
		fast = True
		
# build

class Solution:
	projects = []
	targets = []
	
	def __init__(self, solution):
		self.solution = solution

	def build(self, project, target, platform):
		flags = '/nologo'
		
		if not project:
			print 'No project specified.'
			return False

		safe_project = urllib.quote(project.replace('.', '_'))
		flags += ' /t:\"' + safe_project + '\"'
		
		if target:
			flags += ' /p:Configuration=' + target;
			
		if platform:
			flags += ' /p:Platform=' + platform;
		
		try:
			command = '\"' + cpp_compiler + '\" \"' + self.solution + '\" ' + flags
			print command
			
			output = subprocess.check_call(command)
		except subprocess.CalledProcessError:
			print ''
			print 'Could not setup Visual Studio 2010 environment.'
			return False
			
		return True
		
	def build_list(self):
		for target in self.targets:
			for project in self.projects:
				if not self.build(project, target, 'Win32'):
					print ''
					print 'Failed to build \"' + project + '\" on ' + target + '.'
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
		tests.append('ExLibris.TestRelease.exe')
		if not fast:
			tests.append('ExLibris.TestDebug.exe')
	
	solutions.append(s)
	
# build projects
	
if len(solutions) > 0:
	print '--- Executing'
	print ''
	
	for s in solutions:
		s.targets.append('Release')
		
		if not fast:
			s.targets.append('Debug')
		
		s.build_list()
		
		print ''
else:
	Usage()
	PauseAndExit(0)

# run tests

if len(tests) > 0:
	print '--- Running tests'
	
	for t in tests:
		try:
			print ''
			
			command = 'pushd Build && call \"' + t + '\" && popd'
			subprocess.call(command, shell=True)
			print ''
		except subprocess.CalledProcessError:
			print ''
			print 'Could not run tests.'
		
PauseAndExit(0)