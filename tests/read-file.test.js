const xlsx = require('../index')
const path = require('path')

describe('readFile', () => {
    it('should return data', async (done) => {

        const func = (arr) => {
            return arr
        }

        const filepath = path.join(__dirname, 'get-data.xlsx')
        const result = await xlsx.readFile(filepath)
        expect(result).toBeTruthy();
        done();
    })
})
