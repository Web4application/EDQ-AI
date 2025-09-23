import sys, json
from qiskit import QuantumCircuit, Aer, execute

input_file = sys.argv[1]
output_file = sys.argv[2]

with open(input_file, 'r', encoding='utf-8') as f:
    data = json.load(f)

# Quantum probabilistic simulation for each detection
detections = data.get("detections", [])
qc = QuantumCircuit(len(detections), len(detections))
for i, det in enumerate(detections):
    theta = det.get("confidence",0.5) * 3.1415
    qc.ry(theta, i)
for i in range(len(detections)-1):
    qc.cx(i,i+1)
qc.measure(range(len(detections)), range(len(detections)))
backend = Aer.get_backend('qasm_simulator')
counts = list(execute(qc, backend, shots=1).result().get_counts().keys())[0][::-1]

# Attach quantum decisions
for idx, det in enumerate(detections):
    det["quantum_decision"] = counts[idx]

data["detections"] = detections

with open(output_file, 'w', encoding='utf-8') as f:
    json.dump(data, f)
