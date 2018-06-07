const xlsxio = require('../index')
const path = require('path')

describe('node-gyp', () => {
    it('should compile without error', () => {
        const filepath = path.join(__dirname, 'basic.test.xlsx')
        const result = xlsxio.checkLibrary(filepath);
        expect(true).toBeTruthy()

    })
})
