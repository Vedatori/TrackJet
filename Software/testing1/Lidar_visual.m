clear all;
close all;
clc;
pkg load instrument-control
%graphics_toolkit("fltk")
s1 = serial("COM5")
set(s1, 'baudrate', 115200);     % See List Below
set(s1, 'bytesize', 8);        % 5, 6, 7 or 8
set(s1, 'parity', 'n');        % 'n' or 'y'
set(s1, 'stopbits', 1);        % 1 or 2
set(s1, 'timeout', 123);     % 12.3 Seconds as an example here

srl_flush(s1);

lidarDistance = zeros(1, 181);
lidarAngle = linspace(0, pi, 181);

while true
  if(kbhit (1) == 'x')
    break
  endif
  line = ReadToTermination(s1);
  disp(line);
  data = strsplit(line, ",");
  angle = str2num(char(data(1)));
  distance = str2num(char(data(2)));
  lidarDistance(angle + 1) = distance;
  
  nonzDistance = [];
  nonzAngle = [];
  nonzIndices = find(lidarDistance > 0);
  for i = 1:nnz(lidarDistance)
    limitedDistance = min(lidarDistance(nonzIndices(i)), 1000);
    nonzDistance = [nonzDistance limitedDistance];
    nonzAngle = [nonzAngle lidarAngle(nonzIndices(i))];
  end
  if(angle == 0 || angle == 180)
    polar(nonzAngle, nonzDistance);
  end
  
endwhile
srl_close(s1);