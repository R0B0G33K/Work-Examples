'use strict';

module.exports = {
  up: async (queryInterface, Sequelize) => {
    return queryInterface.createTable(
      'keyRings',{
          keyNum: {
              type: Sequelize.INTEGER,
              primaryKey: true
          },
          keyLocation:{
              type: Sequelize.STRING,
              allowNull: false
          },
          available:{
              type: Sequelize.BOOLEAN,
              allowNull: false
          }
      })
  },

  down: async (queryInterface, Sequelize) => {
    await queryInterface.dropTable('keyRings');
  }
};
