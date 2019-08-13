var model = []

var index = 0;

for (var j = 0; j < 8; j++) {
  for (var i = 0; i < 112; i++) {
      model[index++] = {
          point: [ 0, 0, 0 ]
      };
  }
}

console.log(JSON.stringify(model));
