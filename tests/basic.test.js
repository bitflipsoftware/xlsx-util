const xlsxio = require('../index')

describe('node-gyp', () => {
    it('should compile without error', () => {
        const x = xlsxio.checkLibrary();
        expect(true).toBeTruthy()

    })
})
