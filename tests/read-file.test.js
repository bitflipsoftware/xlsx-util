const xlsx = require('../index')
const path = require('path')

describe('readFile', () => {
    it('should return data', async (done) => {

        const func = (arr) => {
            return arr
        }

        const filepath = path.join(__dirname, 'small.xlsx')
        const result = await xlsx.readFile(filepath)
        expect(result).toBeTruthy();
        expect(Array.isArray(result)).toBeTruthy();
        expect(result.length).toBe(4)

        expect(result[0].A).toBe('Hello')
        expect(result[0].B).toBe('World')



        done();
    })
})
