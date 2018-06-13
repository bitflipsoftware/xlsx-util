const xlsx = require('../index')
const path = require('path')

var SegfaultHandler = require('segfault-handler');

SegfaultHandler.registerHandler("crash.log"); // With no argument, SegfaultHandler will generate a generic log file name

// Optionally specify a callback function for custom logging. This feature is currently only supported for Node.js >= v0.12 running on Linux.
SegfaultHandler.registerHandler("crash.log", function(signal, address, stack) {
    // Do what you want with the signal, address, or stack (array)
    // This callback will execute before the signal is forwarded on.
});

// SegfaultHandler.causeSegfault(); // simulates a buggy native module that dereferences NULL

describe('getData', () => {
    it('should return data', async (done) => {

        const func = (arr) => {
            return arr
        }

        const filepath = path.join(__dirname, 'get-data.xlsx')
        const isFirstRowHeaders = true;

        const result = await xlsx.getData(filepath, isFirstRowHeaders, func)
        expect(result).toBeTruthy();
        done();
    })
})


// describe('getData', () => {
//     it('should find headers', async (done) => {
//
//         const func = (arr) => {
//             return arr
//         }
//
//         const filepath = path.join(__dirname, 'with-headers.xlsx')
//         const isFirstRowHeaders = true;
//
//         const result = await xlsx.getData(filepath, isFirstRowHeaders, func)
//         expect(result).toBeTruthy();
//         done();
//     })
// })
