const xlsx = require('../index')
const path = require('path')

describe('readFile', () => {
    it('should return data', async (done) => {
        expect.assertions(13)

        const filepath = path.join(__dirname, 'small.xlsx')
        const result = await xlsx.readFile(filepath)

        expect(result).toBeTruthy();
        expect(Array.isArray(result)).toBeTruthy();
        expect(result.length).toBe(4)

        expect(result[0].A).toBe('Hello')
        expect(result[0].B).toBe('World')

        expect(result[1].A).toBe('1')
        expect(result[1].B).toBe('2')
        expect(result[1].C).toBe('4')

        expect(result[2].A).toBe('5')
        expect(result[2].B).toBe('6')

        expect(result[3].A).toBe('pp')
        expect(result[3].B).toBe('or')
        expect(result[3].C).toBe('8.5714285714285715E-2')

        done();
    })
})
