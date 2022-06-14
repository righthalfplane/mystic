static int checkStack(pushpopPtr t)
{
    unsigned char *stackData;
    long stackMax;

	if(!t)return 1;

	if(t->stackCount+1 < t->stackMax)return 0;

	stackMax = t->stackMax+STACK_INCREMENT;

	stackData=NULL;
	if(t->stackData){
	    stackData=(unsigned char *)cRealloc((char *)t->stackData,stackMax*t->stackDataSize,8269);
	    if(!stackData){
	        goto ErrorOut;
	    }
	    zerol((char *)&stackData[t->stackMax*t->stackDataSize],STACK_INCREMENT*t->stackDataSize);
	}else{
	    stackData=(unsigned char *)cMalloc(stackMax*t->stackDataSize,8184);
	    if(!stackData){
	        goto ErrorOut;
	    }
	    zerol((char *)stackData,stackMax*t->stackDataSize);
	}

	t->stackMax=stackMax;
	t->stackData=stackData;
	return 0;
ErrorOut:
	return 1;
}
