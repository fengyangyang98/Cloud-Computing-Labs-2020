for((i=1;i<=10;i++)); 
do  
        ab -g $((i*100))${USER}.log -n $((i*2000)) -c $((i*100)) -r -k http://114.116.93.179:2367/
done
