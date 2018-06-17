const xlsx = require('../index')
const path = require('path')

describe('readFile', () => {
    it('should return data', async (done) => {
        expect.assertions(14)

        const filepath = path.join(__dirname, 'small.xlsx')
        const result = await xlsx.readFile(filepath)

        expect(result).toBeTruthy();
        expect(Array.isArray(result)).toBeTruthy();
        expect(result.length).toBe(4)

        expect(result[0].A).toBe('Hello')
        expect(result[0].B).toBe('World')

        expect(result[1].A).toBe(1)
        expect(result[1].B).toBe(2)
        expect(result[1].C).toBe(4)

        expect(result[2].A).toBe(5.5)
        expect(result[2].B).toBe(6)
        expect(result[2].C).toBeNull()

        expect(result[3].A).toBe('pp')
        expect(result[3].B).toBe('or')
        expect(result[3].C).toBeCloseTo(0.07792207792207792, 10)

        done();
    })
})

describe('readFile', () => {
    it('should throw on bad filename', async (done) => {
        expect.assertions(1)

        const filepath = path.join(__dirname, 'badfilename')
        let error = null
        await xlsx.readFile(filepath).then().catch((err)=>{error=err})
        expect(error).toBeTruthy()
        done();
    })
})


describe('readFileWithHeaders', () => {
    it('should return data', async (done) => {
        expect.assertions(12)

        const filepath = path.join(__dirname, 'small.xlsx')
        const result = await xlsx.readFileWithHeaders(filepath)

        expect(result).toBeTruthy();
        expect(Array.isArray(result)).toBeTruthy();
        expect(result.length).toBe(3)

        let r = 0
        expect(result[r].Hello).toBe(1)
        expect(result[r].World).toBe(2)
        expect(result[r].C).toBe(4)

        r = 1
        expect(result[r].Hello).toBe(5.5)
        expect(result[r].World).toBe(6)
        expect(result[r].C).toBeNull()

        r = 2
        expect(result[r].Hello).toBe('pp')
        expect(result[r].World).toBe('or')
        expect(result[r].C).toBeCloseTo(0.07792207792207792, 10)

        done();
    })
})


describe('readFileWithHeaderTransform', () => {
    it('should return data', async (done) => {
        expect.assertions(12)

        const filepath = path.join(__dirname, 'small.xlsx')

        const transform = {
            Hello: 'Bones',
            World: 'Bish',
            C: 'Fish'
        }

        const result = await xlsx.readFileWithHeaderTransform(filepath, transform)

        expect(result).toBeTruthy();
        expect(Array.isArray(result)).toBeTruthy();
        expect(result.length).toBe(3)

        let r = 0
        expect(result[r].Bones).toBe(1)
        expect(result[r].Bish).toBe(2)
        expect(result[r].Fish).toBe(4)

        r = 1
        expect(result[r].Bones).toBe(5.5)
        expect(result[r].Bish).toBe(6)
        expect(result[r].Fish).toBeNull()

        r = 2
        expect(result[r].Bones).toBe('pp')
        expect(result[r].Bish).toBe('or')
        expect(result[r].Fish).toBeCloseTo(0.07792207792207792, 10)

        done();
    })
})


describe('readFileWithHeaderTransformAndDelete', () => {
    it('should return data', async (done) => {
        expect.assertions(12)

        const filepath = path.join(__dirname, 'small.xlsx')

        const transform = {
            Hello: 'Bones',
            World: 'Bish',
            C: 'Fish'
        }

        const deletes = ['World']

        const result = await xlsx.readFileWithHeaderTransformAndDelete(filepath, transform, deletes)

        expect(result).toBeTruthy();
        expect(Array.isArray(result)).toBeTruthy();
        expect(result.length).toBe(3)

        let r = 0
        expect(result[r].Bones).toBe(1)
        expect(result[r].Bish).toBeUndefined()
        expect(result[r].Fish).toBe(4)

        r = 1
        expect(result[r].Bones).toBe(5.5)
        expect(result[r].Bish).toBeUndefined()
        expect(result[r].Fish).toBeNull()

        r = 2
        expect(result[r].Bones).toBe('pp')
        expect(result[r].Bish).toBeUndefined()
        expect(result[r].Fish).toBeCloseTo(0.07792207792207792, 10)

        done();
    })
})
