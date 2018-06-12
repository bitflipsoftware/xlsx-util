const xlsx = require('../index')
const path = require('path')

describe('getData', () => {
    it('should return data', async (done) => {
        const filepath = path.join(__dirname, 'get-data.xlsx')
        const isFirstRowHeaders = true;

        const options = {
            // headerTransform: (headersArray) => {
            //     headersArray.forEach( (oldHeader, index, arr) => {
            //         if( oldHeader === 'hello') {
            //             arr[index] = 'goodbye'
            //         }
            //     })
            //
            //     return headersArray
            // }
            headerTransform: null
        }

        const result = await xlsx.getData(filepath, isFirstRowHeaders, options)
        expect(result).toBeTruthy();
        done();
    })
})


describe('getData', () => {
    it('should find headers', async (done) => {
        const filepath = path.join(__dirname, 'with-headers.xlsx')
        const isFirstRowHeaders = true;

        const options = {
            headerTransform: (headersArray) => {
                headersArray.forEach( (oldHeader, index, arr) => {
                    if( oldHeader === 'Mabel') {
                        arr[index] = 'Siamese'
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
