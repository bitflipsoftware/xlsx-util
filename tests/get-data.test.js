const xlsx = require('../index')
const path = require('path')

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


describe('getData', () => {
    it('should find headers', async (done) => {

        const func = (arr) => {
            return arr
        }

        const filepath = path.join(__dirname, 'with-headers.xlsx')
        const isFirstRowHeaders = true;

        const result = await xlsx.getData(filepath, isFirstRowHeaders, func)
        expect(result).toBeTruthy();
        done();
    })
})
