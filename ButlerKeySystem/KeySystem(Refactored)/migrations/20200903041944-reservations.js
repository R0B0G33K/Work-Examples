'use strict';

module.exports = {
  up: async (queryInterface, Sequelize) => {
    return queryInterface.createTable(
      'reservations',{
          keyLocation: {
              type: Sequelize.STRING,
              primaryKey: true
          },
          personID:{
              type: Sequelize.INTEGER,
              primaryKey: true
          },
          reservedDate:{
              type: Sequelize.DATE,
              primaryKey: true
          },
          reason: {
              type: Sequelize.STRING
          }
      })
  },

  down: async (queryInterface, Sequelize) => {
    await queryInterface.dropTable('reservations');
  }
};
