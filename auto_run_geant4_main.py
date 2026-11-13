import subprocess
import numpy as np
import time

# %% Settings

# Detector settings
# in cm
detector_diameter = 7.62/2
detector_length = 6.31
# Gamma rays to simulate
# in MeV
low_energy = 0
high_energy = 3
energy_bins = 2048
energies = np.linspace(low_energy, high_energy, energy_bins+1)
energies = energies[:-1] + np.diff(energies)/2

# in cm (remember to stay outside of the aluminium can too!)
source_radius = np.sqrt(detector_length**2 + detector_diameter**2)

# Number of simulated decays per setting
runs_per_setting = 4e6

# Other settings
build_folder = "build/"
macro_name = "autorun.mac"
output_folder = "output/"
number_of_threads = 18

# %% Run
start_time = time.time()

print("Starting geant4 autorun")
print("Number of threads: " + str(number_of_threads))
print("Settings: ")
print("\tenergies: " + str(energies))
print("\tsource radii: " + str(source_radius))
print("\truns per setting: " + str(int(runs_per_setting)))

settings = [(ene) 
            for ene in energies]

for i, (energy) in enumerate(settings):
    print(f"Running setting {i + 1} of {len(settings)}, energy={energy}, radius={source_radius}: ")

    # if i <= 1015:
    #     print("\tSkipping setting (already done)")
    #     continue

    settings_name = "_E_" + str(energy)
    settings_name += "_rad_" + str(source_radius)
    settings_name += "_runs_" + f"{runs_per_setting:.1e}"
    settings_name += "_.root"

    # Make macro file first
    macro_content = ""
    macro_content += "/run/numberOfThreads " + str(number_of_threads) + "\n"
    file_name = output_folder + "threadoutput_" + str(i) + settings_name
    
    macro_content += "/E_file_settings/fileName " + file_name + "\n"
    macro_content += "/run/reinitializeGeometry" + "\n"
    macro_content += "/run/initialize" + "\n"
    macro_content += "/E_source/gammaEnergy " + str(energy) + "\n"
    macro_content += "/E_source/sourceRadius " + str(source_radius) + "\n"
    macro_content += "/run/printProgress " + str(int(runs_per_setting/10)) + "\n"
    macro_content += "/run/beamOn " + str(int(runs_per_setting))

    print("\tWriting macro file...")

    with open(build_folder + macro_name, "w") as file:
        file.write(macro_content)

    print("\tRunning Geant4...")

    process_geant4 = [build_folder + "sim", build_folder + macro_name]
    result = subprocess.run(process_geant4, stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)
    # result = subprocess.run(process_geant4)

    print("\tCombining ROOT files...")
    output_file = output_folder + "output_" + str(i) + settings_name
    process_root = "hadd -f " + output_file + " " + output_folder + "threadoutput_" + str(i) + "*.root"
    result = subprocess.run(process_root, shell=True, stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)
    # result = subprocess.run(process_root, shell=True)

    print("\tDeleting temporary ROOT files...")
    process_delete = "rm " + output_folder + "threadoutput_" + str(i) + "*.root"
    # result = subprocess.run(process_delete, stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)
    result = subprocess.run(process_delete, shell=True)

    print("\tFinsihed setting!")

print("All finished!")

end_time = time.time()  # End timing
elapsed_minutes = (end_time - start_time) / 60
print(f"Total time spent: {elapsed_minutes:.2f} minutes")
