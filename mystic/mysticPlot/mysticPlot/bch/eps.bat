clear

filelist open d "/Users/dir/fe/dyna2d96/ibar/ibarOneFine"
eps.s2d
end

filelist open s "/Users/dir/sage/ibar/ibarOneFine"
ibarOneFine-dmp000000   ibarOneFine-dmp000037   ibarOneFine-dmp000066   ibarOneFine-dmp000095   ibarOneFine-dmp000124   
ibarOneFine-dmp000153   ibarOneFine-dmp000182   ibarOneFine-dmp000210   ibarOneFine-dmp000239   ibarOneFine-dmp000268   
ibarOneFine-dmp000297   ibarOneFine-dmp000326   ibarOneFine-dmp000355   ibarOneFine-dmp000383   ibarOneFine-dmp000412   
ibarOneFine-dmp000441   ibarOneFine-dmp000470   ibarOneFine-dmp000499   ibarOneFine-dmp000528   ibarOneFine-dmp000557   
ibarOneFine-dmp000585   ibarOneFine-dmp000614   ibarOneFine-dmp000643   ibarOneFine-dmp000672   ibarOneFine-dmp000701   
ibarOneFine-dmp000730   ibarOneFine-dmp000758   ibarOneFine-dmp000787   ibarOneFine-dmp000816   ibarOneFine-dmp000845   
ibarOneFine-dmp000874   ibarOneFine-dmp000903   ibarOneFine-dmp000931   ibarOneFine-dmp000960   ibarOneFine-dmp000989   
ibarOneFine-dmp001018   ibarOneFine-dmp001047   ibarOneFine-dmp001076   ibarOneFine-dmp001104   ibarOneFine-dmp001133   
ibarOneFine-dmp001162   ibarOneFine-dmp001191   ibarOneFine-dmp001220   ibarOneFine-dmp001249   ibarOneFine-dmp001277   
ibarOneFine-dmp001306   ibarOneFine-dmp001335   ibarOneFine-dmp001364   ibarOneFine-dmp001393   ibarOneFine-dmp001422   
ibarOneFine-dmp001450   
end

filelist create both "/Users/dir/fe/dyna2d96/ibar/ibarOneFine"

filelist set both
outputSize 450 225
outputName "epsBoth.s2d"
end


expression
{
    areaplotset(s,0,0,0, 2.25,2.25,0,225,225,1);
    areaplotset(d,0,0,0, 2.25,2.25,0,225,225,1);
    window("diff",size(225,225));

    if(s:density > 1.0){
        outs=s:eps;
        outd=d;
    }else{
        outs= 0;
        outd= 0;
    }
   maxf=max(fabs(outd));
   diff=100*fabs(outs-outd)/maxf;
   print("maximum percentage difference %g\n",max(diff));
   both=insert(both,xflip(outd),0,0);
   both=insert(both,outs,225,0);
   plotsetrange(both,"eps",s,-2.25,0,0,2.25,2.25,0);

}
step s
end
filelist close both save
filelist close s
filelist close d
exit
