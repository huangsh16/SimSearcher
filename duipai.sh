while true; do
python a.py
./main dict.txt query.txt 0.out
./main1 dict.txt query.txt 1.out
if diff 0.out 1.out; then
echo AC
else
echo WA
exit 0
fi
done
