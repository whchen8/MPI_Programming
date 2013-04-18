@echo off

@rem cmdLine will store the arguments to this bat file, without the first two arguments.
@rem The first two arguments are the mpishim path and the computer name. Those arguments
@rem are already in the string below.
set cmdLine=
shift
shift
:VarLoop
  if (%1)==() goto VarLoopEnd
  set cmdLine=%cmdLine%%1 
  shift
goto VarLoop
:VarLoopEnd

"D:\PDT\OpenMPI_1.6.1\bin\mpiexec.exe" -n 9 -wdir "D:\PDT\Projects\CollectiveCommunication\interalltoallw\Administrator\interalltoallw" -mca btl_tcp_if_exclude lo D:\PDT\Projects\CollectiveCommunication\Debug\interalltoallw.exe "C:\Program Files (x86)\Microsoft Visual Studio 10.0\Common7\IDE\Remote Debugger\x86\mpishim100.exe" 58921 127.0.0.1
