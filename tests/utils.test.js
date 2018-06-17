const xlsx = require('../index')
const path = require('path')

describe('toLower', () => {
    it('should make the ascii string lowercase', () => {
        expect.assertions(1)
        const input = 'aBCdeFG'
        const expected = 'abcdefg'
        const actual = xlsx.toLower(input)
        expect(actual).toBe(expected)
    })
})


describe('replaceAll', () => {
    it('should replace strings', () => {
        expect.assertions(1)
        const input = 'thestrxstr'
        const findStr = 'str'
        const replaceStr = 'Str'
        const expected = 'theStrxStr'
        const actual = xlsx.replaceAll(input, findStr, replaceStr )
        expect(actual).toBe(expected)
    })
})

describe('replaceAll', () => {
    it('should be case insensitive when finding', () => {
        expect.assertions(1)
        const input = 'thesTrxstR'
        const findStr = 'str'
        const replaceStr = 'Str'
        const expected = 'theStrxStr'
        const actual = xlsx.replaceAll(input, findStr, replaceStr )
        expect(actual).toBe(expected)
    })
})
