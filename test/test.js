// class Vector2 {
//   constructor(x, y) {
//     this.x = x;
//     this.y = y;
//   }

//   dot(other) {
//     return this.x * other.x + this.y * other.y;
//   }
// }

// function Shuffle(arrayToShuffle) {
//   for (let e = arrayToShuffle.length - 1; e > 0; e--) {
//     const index = Math.round(Math.random() * (e - 1));
//     const temp = arrayToShuffle[e];

//     arrayToShuffle[e] = arrayToShuffle[index];
//     arrayToShuffle[index] = temp;
//   }
// }

// function MakePermutation() {
//   const permutation = [];
//   for (let i = 0; i < 256; i++) {
//     permutation.push(i);
//   }

//   Shuffle(permutation);

//   for (let i = 0; i < 256; i++) {
//     permutation.push(permutation[i]);
//   }

//   return permutation;
// }
// const Permutation = MakePermutation();

// function GetConstantVector(v) {
//   // v is the value from the permutation table
//   const h = v & 3;
//   if (h == 0)
//     return new Vector2(1.0, 1.0);
//   else if (h == 1)
//     return new Vector2(-1.0, 1.0);
//   else if (h == 2)
//     return new Vector2(-1.0, -1.0);
//   else
//     return new Vector2(1.0, -1.0);
// }

// function Fade(t) {
//   return ((6 * t - 15) * t + 10) * t * t * t;
// }

// function Lerp(t, a1, a2) {
//   return a1 + t * (a2 - a1);
// }

// //https://rtouti.github.io/graphics/perlin-noise-algorithm
// function Noise2D(x, y) {
//   const X = Math.floor(x) & 255;
//   const Y = Math.floor(y) & 255;

//   const xf = x - Math.floor(x);
//   const yf = y - Math.floor(y);

//   const topRight = new Vector2(xf - 1.0, yf - 1.0);
//   const topLeft = new Vector2(xf, yf - 1.0);
//   const bottomRight = new Vector2(xf - 1.0, yf);
//   const bottomLeft = new Vector2(xf, yf);

//   // Select a value from the permutation array for each of the 4 corners
//   const valueTopRight = Permutation[Permutation[X + 1] + Y + 1];
//   const valueTopLeft = Permutation[Permutation[X] + Y + 1];
//   const valueBottomRight = Permutation[Permutation[X + 1] + Y];
//   const valueBottomLeft = Permutation[Permutation[X] + Y];

//   const dotTopRight = topRight.dot(GetConstantVector(valueTopRight));
//   const dotTopLeft = topLeft.dot(GetConstantVector(valueTopLeft));
//   const dotBottomRight = bottomRight.dot(GetConstantVector(valueBottomRight));
//   const dotBottomLeft = bottomLeft.dot(GetConstantVector(valueBottomLeft));

//   const u = Fade(xf);
//   const v = Fade(yf);

//   return Lerp(u,
//     Lerp(v, dotBottomLeft, dotTopLeft),
//     Lerp(v, dotBottomRight, dotTopRight)
//   );
// }

const perm = [151,160,137,91,90,15,
  131,13,201,95,96,53,194,233,7,225,140,36,103,30,69,142,8,99,37,240,21,10,
  23,190, 6,148,247,120,234,75,0,26,197,62,94,252,219,203,117,35,11,32,57,
  177,33,88,237,149,56,87,174,20,125,136,171,168, 68,175,74,165,71,134,139,
  48,27,166,77,146,158,231,83,111,229,122,60,211,133,230,220,105,92,41,55,
  46,245,40,244,102,143,54, 65,25,63,161,1,216,80,73,209,76,132,187,208,89,
  18,169,200,196,135,130,116,188,159,86,164,100,109,198,173,186,3,64,52,217,
  226,250,124,123,5,202,38,147,118,126,255,82,85,212,207,206,59,227,47,16,58,
  17,182,189,28,42,223,183,170,213,119,248,152,2,44,154,163,70,221,153,101,
  155,167,43,172,9,129,22,39,253,19,98,108,110,79,113,224,232,178,185,112,
  104,218,246,97,228,251,34,242,193,238,210,144,12,191,179,162,241,81,51,
  145,235,249,14,239,107,49,192,214,31,181,199,106,157,184,84,204,176,115,
  121,50,45,127,4,150,254,138,236,205,93,222,114,67,29,24,72,243,141,128,195,
  78,66,215,61,156,180];

// Duplicate the permutation table to avoid overflow
const p = perm.concat(perm);

// Fade function
function fade(t) {
  return t * t * t * (t * (t * 6 - 15) + 10);
}

// Linear interpolation
function lerp(t, a, b) {
  return a + t * (b - a);
}

// Gradient function for Perlin noise
function grad(hash, x, y, z) {
  const h = hash & 15;
  const u = h < 8 ? x : y;
  const v = h < 4 ? y : (h === 12 || h === 14 ? x : z);
  return ((h & 1) ? -u : u) + ((h & 2) ? -v : v);
}

// Perlin noise function
function perlin(x, y, z) {
  const X = Math.floor(x) & 255;
  const Y = Math.floor(y) & 255;
  const Z = Math.floor(z) & 255;

  x -= Math.floor(x);
  y -= Math.floor(y);
  z -= Math.floor(z);

  const u = fade(x);
  const v = fade(y);
  const w = fade(z);

  const A = p[X] + Y;
  const AA = p[A] + Z;
  const AB = p[A + 1] + Z;
  const B = p[X + 1] + Y;
  const BA = p[B] + Z;
  const BB = p[B + 1] + Z;

  return lerp(w, lerp(v, lerp(u, grad(p[AA], x, y, z),
                              grad(p[BA], x - 1, y, z)),
                      lerp(u, grad(p[AB], x, y - 1, z),
                           grad(p[BB], x - 1, y - 1, z))),
             lerp(v, lerp(u, grad(p[AA + 1], x, y, z - 1),
                          grad(p[BA + 1], x - 1, y, z - 1)),
                  lerp(u, grad(p[AB + 1], x, y - 1, z - 1),
                       grad(p[BB + 1], x - 1, y - 1, z - 1))));
}


const canvas = document.getElementById("myCanvas")
const ctx = canvas.getContext("2d")
const height = 500
const width = 500

const check_size = 100
const pixel_size = width / check_size

ctx.fillStyle = "white";
ctx.fillRect(0, 0, canvas.width, canvas.height);

let size = 1
let time = 1

document.getElementById("myRange1").addEventListener("mouseup", (e) => {
  size = e.target.value / 100
  console.log(size)
  make_map()
})

document.getElementById("myRange2").addEventListener("input", (e) => {
  time = e.target.value / 1000
  console.log(time)
  make_map()
})

const make_map = () => {
  for (let y = 0; y < check_size; y++) {
    for (let x = 0; x < check_size; x++) {
      const value = perlin(x / check_size * size,y / check_size* size,time)
      
      const white = Math.floor((value + 1) * 127.5)
      ctx.fillStyle = `rgb(${white},${white},${white})`;
  
      ctx.fillRect(pixel_size * y, pixel_size * x, pixel_size * (y + 1), pixel_size * (x + 1));
    }
  }
}


make_map()