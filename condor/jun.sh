./condor-LYSquareTrigger_CMSSW.sh -x -10.0 -z 0.0 -X 40.0 -Y 40.0 -l 200.0 -w 10.0 -f 0.6 -s 0.0 -a 400.0 --tiledecay 3.945 --tilerise 0.5 -b 5000.0 -y 10000.0 -m 0.985 -d 7.193 -e 1.0 -P 1 -H 0 -c 1 -r 1.0 -R 4.0 -N 50 -o test_propagatetoend_goodcore_ALLtrans_refrac_1p0.root

./condor-LYSquareTrigger_CMSSW.sh -x -0.0 -z 0.0 -X 40.0 -Y 40.0 -l 200.0 -w 10.0 -f 0.6 -s 0.0 -a 400.0 --tiledecay 3.945 --tilerise 0.5 -b 5000.0 -y 10000.0 -m 0.985 -d 7.193 -e 1.0 -P 1 -H 0 -c 1 -r 1.0 -R 4.0 -N 10 -o test_propagatetoend_goodcore_clad_refrac 1

# test clad dirt

python3 SubmitJobs.py -x 0 -z 0 -X 40.0 -Y 10 -l 500 -w 10.0 -y 10000  -N 50 -e 1 -P 1 -H 0 -a 400 -b 5000 --tiledecay 3.945 -d 7.193  -s 0 -c 1 -f 0.6 -r 1 1.2 1.4 1.6 1.8 -R 2 -m 0.985 -S 0.001 0.01 0.05 0.1 

# 1x4x50


# Tx4x50 cm, 2 radius hole, 0.7 radius core
python3 SubmitJobs.py -x $(seq -20 2 20) -z 0 -X 40.0 -Y $(seq 10 10 40) -l 500 -w 10.0 -y 10000  -N 50 -e 1 -P 1 -H 0 -a 400 -b 5000 --tiledecay 3.945 -d 7.193  -s 0 -c 1 -f 0.6 -r 1.0 -R 2 -m 0.985

# Different glue 1x4x20 cm, 2 radius hole, 0.7 radius core 
python3 SubmitJobs.py -x $(seq -20 2 20) -z 0 -X 40.0 -Y 10.0 -l 500 -w 10.0 -y 10000  -N 50 -e 1 -P 1 -H 0 -a 400 -b 5000 --tiledecay 3.945 -d 7.193  -s 0 -c 1 -f 0.6 -r 1.0 1.2 1.4 1.6 1.8 2.0 -R 2 -m 0.985


# different hole shape
python3 SubmitJobs.py -x $(seq -20 2 20) -z 0 -X 40.0 -Y 40.0 -l 200 -w 10.0 -y 10000  -N 50 -e 1 -P 1 -H 0 -a 400 -b 5000 --tiledecay 3.945 -d 7.193  -s -3.25 3.25 -c 0 -f 0.6 -r 1.0 1.4 -R 4 -m 0.985
python3 SubmitJobs.py -x $(seq -20 2 20) -z 0 -X 40.0 -Y 40.0 -l 200 -w 10.0 -y 10000  -N 50 -e 1 -P 1 -H 0 -a 400 -b 5000 --tiledecay 3.945 -d 7.193  -s 0 -c 1 -f 0.6 -r 1.0 1.2 1.4 1.6 1.8 2.0 -R 4 -m 0.985

# shift the fiber horizontally
python3 SubmitJobs.py -x $(seq -20 2 20) -z 0 -X 40.0 -Y 40.0 -l 200 -w 10.0 -y 10000  -N 50 -e 1 -P 1 -H 0 -a 400 -b 5000 --tiledecay 3.945 -d 7.193  -s -3.25 0 3.25 -f 0.6 -r 1.0 1.4 -R 4 -m 0.985


# Second, glue with 1.4 refraction 
python3 SubmitJobs.py -x $(seq -20 2 20) -z 0 -X 40.0 -Y 10.0 -l 140 -w 10.0 -y 10000  -N 50 -e 1 -P 1 -H 0 -a 400 -b 5000 --tiledecay 3.945 -d 7.193  -s 0 -f 0.6 -r 1.4 -R 1.5 -m 0.985
python3 SubmitJobs.py -x $(seq -20 2 20) -z 0 -X 40.0 -Y 40.0 -l 200 -w 10.0 -y 10000  -N 50 -e 1 -P 1 -H 0 -a 400 -b 5000 --tiledecay 3.945 -d 7.193  -s 0 -f 0.6 -r 1.0 1.2 1.4 1.6 1.8 2.0 -R 4 -m 0.985

# First, with air
python3 SubmitJobs.py -x $(seq -20 2 20) -z 0 -X 40.0 -Y 10.0 -l 140 -w 10.0 -y 10000  -N 50 -e 1 -P 1 -H 0 -a 400 -b 5000 --tiledecay 3.945 -d 7.193  -s 0 -f 0.6 -r 1.0 -R 1.5 -m 0.985
python3 SubmitJobs.py -x $(seq -20 2 20) -z 0 -X 40.0 -Y 40.0 -l 200 -w 10.0 -y 10000  -N 50 -e 1 -P 1 -H 0 -a 400 -b 5000 --tiledecay 3.945 -d 7.193  -s 0 -f 0.6 -r 1.0 -R 1 2 4 -m 0.985



# Hole size study
python3 SubmitJobs.py -x $(seq -20 2 20) -z 0 -X 40.0 -Y 40.0 -l 500.0 -w 2.0 -y 10000  -N 50 -e 1 -P 1 -H 0 -a 80 -b 5000 --tiledecay 3.945 -d 7.193  -s 0 -f 0.6 -r  1.0 -R 1 2 4 -m 0.985

# epoxy refraction index
# length 
python3 SubmitJobs.py -x $(seq -20 2 20) -z 0 -X 40.0 -Y 40.0 -l 100.0 200.0 300.0 400.0 500.0 -w 2.0 -y 10000  -N 50 -e 1 -P 1 -H 0 -a 80 -b 5000 --tiledecay 3.945 -d 7.193  -s 0 -f 0.6 -r 1.0 -R 1 -m 0.985

python3 SubmitJobs.py -x 0 -z 0 -X 40.0 -Y 40.0 -l 100.0 200.0 300.0 400.0 500.0 -w 10.0 -y 10000  -N 50 -e 1 -P 1 -H 0 -a 80 160 240 320 400 480 560 640 720 -b 5000 --tiledecay 3.945 -d 7.193  -s 0 -f 0.6 -r 1.0 -R 1 -m 0.985

python3 SubmitJobs.py -x 0 -z 0 -X 40.0 -Y 40.0 -l 100.0 200.0 300.0 400.0 500.0 -w 10.0 -y 10000  -N 50 -e 1 -P 1 -H 0 -a 800 880 960 1040 1120 1200 -b 5000 --tiledecay 3.945 -d 7.193  -s 0 -f 0.6 -r 1.0 -R 1 -m 0.985

python3 SubmitJobs.py -x 0 -z 0 -X 40.0 -Y 10.0 -l 100.0 200.0 300.0 400.0 500.0 -w 10.0 -y 10000  -N 50 -e 1 -P 1 -H 0 -a 80 240 400 560 720 880 1040 1200 -b 5000 --tiledecay 3.945 -d 7.193  -s 0 -f 0.6 -r 1.0 -R 1 -m 0.985






python3 SubmitJobs.py -x $(seq -20 2 20) -z 0 -X 40.0 -Y 40.0 -l 200.0 -w 2.0 -y 10000  -N 50 -e 1 -P 1 -H 0 -a 80 -b 5000 --tiledecay 3.945 -d 7.193  -s 0 -f 0.3 -r  1.0 1.4 1.53 1.65 2.0  -R 4 -m 0.985 #borrow -r for refraction


python3 SubmitJobs.py -x $(seq -20 2 20) -z 0 -X 40.0 -Y 40.0 -l 200.0 -w 2.0 -y 10000  -N 50 -e 1 -P 1 -H 0 -a 80 -b 5000 --tiledecay 3.945 -d 7.193  -s 0 -f 0.3 -r  $(seq 1.4 0.05 1.6) 1.53  -R 4 -m 0.985 #borrow -r for refraction
