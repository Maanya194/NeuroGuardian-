***NeuroGuardian: Real-Time Cognitive Load Monitoring System for Accident Prevention***


NeuroGuardian is an intelligent real-time driver monitoring system designed to detect cognitive load, fatigue, and reduced alertness using EEG signals, machine learning, and IoT-based feedback mechanisms.
The main objective is to prevent accidents by identifying mental fatigue early and providing timely warnings to drivers or vehicle systems.


***Overview:***

Driver fatigue is one of the major causes of road accidents worldwide. Traditional fatigue monitoring methods (like camera-based face tracking) often fail in low-light or obstructed conditions.

NeuroGuardian solves this by using EEG brainwave activity to understand a driver's cognitive state with greater accuracy. The system analyzes EEG signals, identifies patterns indicating fatigue or high cognitive load, and triggers real-time alerts.


***Key Features:***

✔️ Real-time EEG signal acquisition using a MindWave/EEG sensor<br>
✔️ Arduino-based signal preprocessing and Bluetooth communication<br>
✔️ Noise filtering (Fourier + Bandpass Filters)<br>
✔️ Feature extraction for Alpha, Beta, Theta brainwave bands<br>
✔️ Machine learning classification (Alert vs Fatigued vs Overloaded)<br>
✔️ UI dashboard for visualizing driver state<br>
✔️ Real-time alert mechanism (buzzer / LED / notification)<br>
✔️ Data logging for long-term behavior monitoring


***Hardware Components:***

EEG MindWave / Brainwave sensor<br>
Arduino Uno / Nano<br>
HC-05 / HC-06 Bluetooth Module<br>
Buzzer + LED Indicators<br>
Laptop/PC for ML Processing


***Software & ML Components:***

Python<br>
NumPy, Pandas<br>
SciPy for signal filtering<br>
Matplotlib for visualization<br>
Scikit-learn / TensorFlow classifier<br>
Arduino IDE<br>
Serial/Bluetooth communication<br>
Cloud database (optional: Firebase / MongoDB)


***Data Flow (End-to-End):***

1. EEG sensor collects raw brainwaves
2. Arduino receives & preprocesses data
3. Data sent to Python over Bluetooth
4. Noise removal + band extraction
5. ML model classifies cognitive state
6. If fatigue detected → Send alert
7. Logs stored for analysis


***Machine Learning Approach:***

**1. Preprocessing**

Bandpass filter (0.5–60 Hz)<br>
Notch filter to remove 50/60 Hz noise<br>
Smoothing

**2. Feature Extraction**

Power Spectral Density (PSD)<br>
Alpha waves → relaxation<br>
Beta waves → alert mental activity<br>
Theta waves → drowsiness

**3. Classification**

Trained ML models tested:<br>

SVM<br>
Random Forest<br>
Logistic Regression<br>

Final chosen model: Random Forest (best accuracy/performance for this dataset)


***Results:***

Accuracy achieved: 85–92% depending on model<br>
Stable classification during real-time testing<br>
System successfully triggered alerts during fatigue simulation


