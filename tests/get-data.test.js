const xlsx = require('../index')
const path = require('path')

describe('getSheetNames', () => {
    it('should return data', async (done) => {
        const filepath = path.join(__dirname, 'get-data.xlsx')
        const isFirstRowHeaders = false;

        const options = {
            headerTransform: (headersArray) => {
                headersArray.forEach( (oldHeader, index, arr) => {
                    if( oldHeader === 'hello') {
                        arr[index] = 'goodbye'
                    }
                })

                return headersArray
            }
        }

        const result = await xlsx.getData(filepath, isFirstRowHeaders, options)
        expect(result).toBeTruthy();
        done();
    })
})


describe('getSheetNames', () => {
    it('should find headers', async (done) => {
        const filepath = path.join(__dirname, 'with-headers.xlsx')
        const isFirstRowHeaders = true;

        const options = {
            headerOverrides: {
                hello: "goodbye"
            }
        }

        const result = await xlsx.getData(filepath, isFirstRowHeaders, options)
        expect(result).toBeTruthy();
        done();
    })
})
