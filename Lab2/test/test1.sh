for((i=1;i<=5;i++))
do  
        ab -g $((i*100))${USER}.log -n $((1500)) -c $((i*100)) -r -k http://114.116.93.179:2367/>>${USER}.log
done
