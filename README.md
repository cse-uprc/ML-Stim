# ML Stim  
This project is built to help bring neuroscience into the 21st century. All current simulation environments are either slow, or written by the neuroscientists themselves, which makes sharing research difficult. We create a simulation environment that could be deployed on cloud computing or high performance computing for easier sharing of data.  
On top of our environment we created a machine learning algorithm to control electrical stimulation of the model. This is then trained to try and block propagation of electrical signals.  
### To run simulation  
```
make
cd bin
./controller.exe
```
This will run the machine learning simulation. The algorithm attempts to find the optimum method for blocking action potentials. The best neural net from each generation is run again, and the data is saved to file.  
