var dht = require('./build/Release/node_dht_edison');
var ret = dht.read(31);
if(ret.valid) {
        console.log('T='+ret.t+ " H="+ret.h);
}
else {
        console.log('error');
}
