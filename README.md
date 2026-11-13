# HPGe_models_b12806_b12810
Geant4 models for two Canberra/Mirion GR1020 HPGe detectors with serial numbers b12806 and b12810. 

Supplied with python script that runs a simulation with a SURE model background source with varying energy, which can of course be modified for different applications. 

In an environment with root and python: 

    mkdir build

    mkdir output

    cd build

    cmake ..

    make -j4

    cd ..

    python3 auto_run_geant4.py
