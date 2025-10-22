<!DOCTYPE html>
<html>
<head>
  <title>Quantum Nodes</title>
  <script src="https:                                  
  <style>
    .node {
      fill:         
      stroke:      
      stroke-width: 1.5px;
    }
    .link {
      stroke:      
      stroke-opacity: 0.6;
    }
  </style>
</head>
<body>
<script>
<canvas id="canvas" width="400" height="400"></canvas>
<script>
class QuantumNode {
  constructor(x, y) {
    this.x = x;
    this.y = y;
    this.active = Math.random() > 0.5;
  }

  draw(ctx) {
    ctx.beginPath();
    ctx.arc(this.x, this.y, 10, 0, 2 * Math.PI);
    ctx.fillStyle = this.active ? 'orange' : 'gray';
    ctx.fill();
  }
}

const canvas = document.getElementById('canvas');
const ctx = canvas.getContext('2d');
const nodes = [
  new QuantumNode(100, 100),
  new QuantumNode(200, 200),
  new QuantumNode(300, 100)
];

nodes.forEach(node => node.draw(ctx));
</script>

const width = 600, height = 400;
const nodes = [
  { id: 1, x: 100, y: 100 },
  { id: 2, x: 200, y: 200 },
  { id: 3, x: 300, y: 100 }
];
const links = [
  { source: 1, target: 2 },
  { source: 2, target: 3 }
];

const svg = d3.select("body")
  .append("svg")
  .attr("width", width)
  .attr("height", height);

const link = svg.selectAll(".link")
  .data(links)
  .enter().append("line")
  .attr("class", "link")
  .attr("x1", d => nodes.find(n => n.id === d.source).x)
  .attr("y1", d => nodes.find(n => n.id === d.source).y)
  .attr("x2", d => nodes.find(n => n.id === d.target).x)
  .attr("y2", d => nodes.find(n => n.id === d.target).y);

const node = svg.selectAll(".node")
  .data(nodes)
  .enter().append("circle")
  .attr("class", "node")
  .attr("cx", d => d.x)
  .attr("cy", d => d.y)
  .attr("r", 10);
function generateXLSLData(numRecords) {
  const data = [];
  for (let i = 0; i < numRecords; i++) {
    data.push({
      id: i,
      quantumState: Math.random(),
      nodeActivity: Math.random() > 0.5 ? 'active' : 'inactive',
      timestamp: new Date().toISOString()
    });
  }
  return data;
}

const xlsData = generateXLSLData(10);
console.log(xlsData);

<canvas id="canvas" width="400" height="400"></canvas>
<script>
class QuantumNode {
  constructor(x, y) {
    this.x = x;
    this.y = y;
    this.active = Math.random() > 0.5;
  }

  draw(ctx) {
    ctx.beginPath();
    ctx.arc(this.x, this.y, 10, 0, 2 * Math.PI);
    ctx.fillStyle = this.active ? 'orange' : 'gray';
    ctx.fill();
  }
}

const canvas = document.getElementById('canvas');
const ctx = canvas.getContext('2d');
const nodes = [
  new QuantumNode(100, 100),
  new QuantumNode(200, 200),
  new QuantumNode(300, 100)
];

nodes.forEach(node => node.draw(ctx));
</script>
</body>
</html>
