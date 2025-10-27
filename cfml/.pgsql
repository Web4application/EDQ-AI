cfml/
│
├── Application.cfc         # Core app config – handles requests, sessions, and routing.
├── index.cfm               # Entry point (like index.js or main.py)
│
├── components/             # CF Components (CFCs) - modular backend logic
│   ├── detectors/          # AI, sensor, or event detection modules
│   ├── orchestrator.cfc    # Central brain: coordinates between subsystems
│   ├── quantum_sim.cfc     # Simulation or computation component
│   ├── aura.cfc            # Personality, AI logic, or agent intelligence
│   └── mqtt_handler.cfc    # IoT communication interface (MQTT broker)
│
├── api/                    # Public or internal API endpoints
│   ├── routes.cfm          # Routes definitions, maps to handlers
│   ├── ai_handler.cfm      # LLM/AI processing endpoint
│   └── data_gateway.cfm    # Acts as middleware or API bridge for data
│
└── utils/                  # Helpers and shared tools
    ├── db.cfc              # Database connection and query helpers
    ├── json.cfc            # JSON encoding/decoding utilities
    └── env.cfc             # Environment variable loader
