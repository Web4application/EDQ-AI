const connectToMQTT = () => {
  const client = mqtt.connect('pf-f8i70zck04uhfsacjqga.cedalo.cloud:8000/mqtt'); // Replace with your broker's WebSocket URL

  client.on('connect', () => {
    console.log('Connected to MQTT broker');
    client.subscribe('machine_topic', (err) => {
      if (!err) console.log('Subscribed to machine_topic');
    });
  });

  client.on('message', (topic, message) => {
    console.log(`Received message on topic ${topic}:`, message.toString());
  });

  client.on('error', (err) => {
    console.error('MQTT Error:', err);
  });

  const sendMQTTMessage = (topic, message) => {
    client.publish(topic, message);
    console.log(`Message sent to topic ${topic}: ${message}`);
  };

  // Example usage
  sendMQTTMessage('machine_topic', 'Hello from Web4!');
};

connectToMQTT();
