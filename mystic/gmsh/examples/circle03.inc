mat 4
1 2 1 1 1 1. 18e6 0.35 0. 2.64
1 2 1 1 1 1. 18e6 0.35 0. 2.64
1 2 1 1 1 1. 36e6 0.35 0. 5.64
1 2 1 1 1 1. 18e6 0.35 0. 2.64
bc
fix curve 1 2
fix curve 4 2
fix curve 3 3
force curve 2 1000. 0. 0.
end

bc
fix curve 1 2
fix curve 4 2
fix curve 3 3
force curve 2 1000. 0. 0.
end


form
solv,stre,1,end
stop


form,1,0,2,2

mar 100 4e-5 1
end
end
end
stop
