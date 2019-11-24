function  plotFile (file)
    x = csvread(file);
    xx = x(:,1);
    yy = x(:,2);
    plot(xx,yy);
endfunction
